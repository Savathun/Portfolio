#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <cuda.h>

#define Max_2d_Block_Dim 32
#define Max_Concurent_Blocks 56
#define Max_Block_Dim 1024
#define HANDLE_ERROR( err )  ( HandleError( err, __FILE__, __LINE__ ) )
// I made this using the stats_gpu_template.cu as a guide 
typedef struct {
    unsigned int index;
    double value;
} local_max;

void HandleError(cudaError_t err, const char* file, int line) {
    //
    // Handle and report on CUDA errors.
    //
    if (err != cudaSuccess) {
        printf("%s in %s at line %d\n", cudaGetErrorString(err), file, line);

        exit(EXIT_FAILURE);
    }
}

void checkCUDAError(const char* msg, bool exitOnError) {
    //
    // Check cuda error and print result if appropriate.
    //
    cudaError_t err = cudaGetLastError();

    if (cudaSuccess != err) {
        fprintf(stderr, "Cuda error: %s: %s.\n", msg, cudaGetErrorString(err));
        if (exitOnError) {
            exit(-37);
        }
    }
}

void cleanupCuda(void) {
    //
    // Clean up CUDA resources.
    //

    //
    // Explicitly cleans up all runtime-related resources associated with the
    // calling host thread.
    //
    HANDLE_ERROR(
        cudaThreadExit()
    );
}

void columnMaxKernelDim(unsigned int& blocks, unsigned int& threads, const unsigned int& size) {
    blocks = (size / Max_Block_Dim) + ((size % Max_Block_Dim) ? 1 : 0);
    threads = Max_Block_Dim;
    // blocks are the numeber of blocks when spliting by Max_Block_Dim. It will add another if necessary 
    // threads are equal to Max_Block_Dim by default
     
    if (size < threads) {
        blocks = 1; // makes clear that only 1 block is desired 
        if (size < 2) threads = 2;
        else if (size < 4) threads = 4;
        else if (size < 8) threads = 8;
        else if (size < 16) threads = 16;
        else if (size < 32) threads = 32;
        else if (size < 64) threads = 64;
        else if (size < 128) threads = 128;
        else if (size < 256) threads = 256;
        else if (size < 512) threads = 512;
        else if (size < 1024) threads = 1024;
    }
    // if there are less elements than there are allotted in single block will limit the threads allocated in the gpu
}

void swapRowsKernelDim(unsigned int& blocks, unsigned int& threads, const unsigned int& size) {
    blocks = (size / Max_Block_Dim) + ((size % Max_Block_Dim) ? 1 : 0);
    threads = Max_Block_Dim;

    if (size < threads){
        threads = size;
    }
}

void gaussianReductionKernelDim(unsigned int& blocks, unsigned int& threads, const unsigned int& size) {
    blocks = (size / Max_2d_Block_Dim) + ((size % Max_2d_Block_Dim) ? 1 : 0);
    threads = Max_2d_Block_Dim;

    if (size < threads){ 
        threads = size; 
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// GPU KERNELS

__device__ double device_abs(double x) {
    //
    // Calculate x^y on the GPU.
    //
    return fabs(x);
}

__device__ double device_invert_sign(double x) {
    // returns the opposite sign of x
    if (x < 0.0) return 1.0;
    else return -1.0;
}

__global__ void columnMax(double* A, unsigned int* size, unsigned int* incolumn, local_max* outdata) {
    extern __shared__ double sdata[];           // container for the data being processed 
    __shared__ unsigned int index_partition;    // beginning for the index partition 
    __shared__ unsigned int N;                  // size of matrix (NxN)
    __shared__ unsigned int column;             // columns we find the max in

    unsigned int tid = threadIdx.x;

    if (tid == 0) {
        index_partition = blockDim.x;
        N = *size;
        column = *incolumn;
    }

    __syncthreads();

    int row = blockDim.x * blockIdx.x + tid;      // the abs position for within the matrix
    if (row >= column && row < N) {
        sdata[tid] = A[row * N + column];     // puts the matrix value into sdata
        sdata[tid + index_partition] = row;        // puts the matrix index into sdata
    }
    else {
        sdata[tid] = 0;     // puts the matrix value into sdata
        sdata[tid + index_partition] = -1;        // puts the matrix index into sdata
    }

    __syncthreads();

    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) {
            double absL = device_abs(sdata[tid]);   // abs value of the left 
            double absR = device_abs(sdata[tid + s]);   // abs value of the right
            if (absL < absR) {
                // if the right num is larger
                sdata[tid] = sdata[tid + s];
                sdata[tid + index_partition] = sdata[tid + s + index_partition];
                // this empties  the right half of the array but is fine because it isn't needed anymore
            }
        }
        __syncthreads();
    }

    if (tid == 0) {
        outdata[blockIdx.x].index = sdata[index_partition];
        outdata[blockIdx.x].value = sdata[0];
    }
}

__global__ void swapRows(double* A, unsigned int* size, unsigned int* top_row, unsigned int* bot_row)
{
    extern __shared__ double sdata[];           // container for the data processing
    __shared__ unsigned int N;                  // size of matrix (NxN)
    __shared__ unsigned int top;                // the top row that is swapped with the bottom row
    __shared__ unsigned int bot;                // the bottom row that is swapped with the top row

    unsigned int tid = threadIdx.x;

    if (tid == 0) {
        N = *size; 
        top = *top_row;
        bot = *bot_row;
    }

    __syncthreads();

    // abs position within the matrix
    int column = blockDim.x * blockIdx.x + tid;

    // only swap the sub matrix A[row][row] because everything above and to the left of row is dealt with
    // above row data in U
    // left of row data in L

    // load the top row into shared memory
    if (column < N) {
        sdata[tid] = A[top * N + column];
        A[top * N + column] = A[bot * N + column];
        A[bot * N + column] = sdata[tid];
    }
}

__global__ void findCoefficients(double* A, unsigned int* size, unsigned int* inboundary) {
    extern __shared__ double sdata[];           // container for processing data
    __shared__ unsigned int N;                  // size of matrix (NxN)
    __shared__ unsigned int boundary;           // the top row that is swapped with the bottom row
    __shared__ double denominator;              // the number to go into the denominator
    double coefficient;

    unsigned int tid = threadIdx.x;

    if (tid == 0) {
        boundary = *inboundary;
        N = *size;
        denominator = A[boundary * N + boundary]; // top left value
    }

    __syncthreads();

    unsigned int row = blockDim.x * blockIdx.x + tid;
    if (row > boundary && row < N) {
        sdata[tid] = A[row * N + boundary];
        coefficient = sdata[tid] / denominator;
        A[row * N + boundary] = coefficient;
    }
}

__global__ void gaussianReduction(double* A, unsigned int* size, unsigned int* inboundary) {
    __shared__ unsigned int N;                  // size of matrix (NxN)
    __shared__ unsigned int boundary;           // the top row that is swapped with the bottom row
    double coefficient;                         // the number for multiplication of the top row (the number in L)
    double R_bot;                               // the element in the bottom row
    double R_top;                               // the number in the top row multipied by coefficient


    unsigned int tidx = threadIdx.x;
    unsigned int tidy = threadIdx.y;
    unsigned int absolute_row = blockDim.x * blockIdx.x + tidx;
    unsigned int absolute_column = blockDim.y * blockIdx.y + tidy;

    if (tidx == 0 && tidy == 0) {
        N = *size;
        boundary = *inboundary;
    }

    __syncthreads();

    if ((absolute_column > boundary && absolute_column < N) && (absolute_row > boundary && absolute_row < N)) {
        coefficient = A[absolute_row * N + boundary];
        coefficient *= device_invert_sign(A[absolute_row * N + absolute_column]);
        R_bot = A[absolute_row * N + absolute_column];
        R_top = A[boundary * N + absolute_column];
        A[absolute_row * N + absolute_column] = R_bot + coefficient * R_top;
    }
}
// ---------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    unsigned int seed;
    struct timeval start, end;
    double runtime;
    bool singular = false;
    bool toPrint = false;

    if (argc < 2) {
        printf("Format: stats_gpu <size of array> <random seed> <print>\n");
        printf("Arguments:\n");
        printf("  size of array - The size of the matrix to be generated and then processed\n");
        printf("                  generator that will generate the contents of the array\n");
        printf("                  to be processed\n");

        exit(1);
    }

    // Get the size of matrix 
    unsigned int array_size = atoi(argv[1]);
    unsigned int array_area = array_size * array_size;

    // Get the print variable
    if (argv[3])
    {
        if (array_size < 10)
        {
            toPrint = atoi(argv[3]);
        }
        else
        {
            toPrint = 0;
        }
    }
    else
    {
        toPrint = 0;
    }


    // Get the seed to be used
    seed = atoi(argv[2]);

    // Make sure that CUDA resources get cleaned up on exit.
    atexit(cleanupCuda);

    // Record the start time.
    gettimeofday(&start, NULL);

    // Allocate the array to be populated
    double* array = (double*)malloc(array_area * sizeof(double));
    printf("Array size: %d\nArray Area: %d\nSeed: %d\n", array_size, array_area, seed);

    // initialize the 2d array/ matrix
    srand(seed);
    for (int i = 0; i < array_size; i++) {
        for (int j = 0; j < array_size; ++j) {
            //printf("[%d]\n", i*array_size + j);
            array[i * array_size + j] = ((double)rand() / (double)RAND_MAX);
        }
    }

    if (toPrint == true)
    {
        printf("Matrix A\n");
        for (int i = 0; i < array_size; i++) {
            printf("Row %d: ", i);
            for (int j = 0; j < array_size; ++j) {
                printf("%.2f\t", array[i * array_size + j]);
            }
            printf("\n");
        }
    }

    // write the matrix address to GPU
    double* A; // for copying array to the GPU
    HANDLE_ERROR(cudaMalloc((void**)&A, array_area * sizeof(double)));
    HANDLE_ERROR(cudaMemcpy(A, array, array_area * sizeof(double), cudaMemcpyHostToDevice));

    // write the array size to GPU
    unsigned int* array_size_ptr;
    HANDLE_ERROR(cudaMalloc((void**)&array_size_ptr, sizeof(unsigned int)));
    HANDLE_ERROR(cudaMemcpy(array_size_ptr, &array_size, sizeof(unsigned int), cudaMemcpyHostToDevice));

    unsigned int* row_to_swap_ptr;
    HANDLE_ERROR(cudaMalloc((void**)&row_to_swap_ptr, sizeof(unsigned int)));

    unsigned int* current_row_ptr;
    HANDLE_ERROR(cudaMalloc((void**)&current_row_ptr, sizeof(unsigned int)));


    // Record the end time.
    gettimeofday(&end, NULL);

    // -----------------------------------------------------------------------------------------------------------------
    // LU Decomp
    // -----------------------------------------------------------------------------------------------------------------

    // kernel variables
    unsigned int numthreads, numblocks, sdata_size; // the number of threads and blocks assigned 


    columnMaxKernelDim(numblocks, numthreads, array_size);
    local_max* max_data = new local_max[numblocks];
    local_max* max_data_ptr;
    HANDLE_ERROR(cudaMalloc((void**)&max_data_ptr, numblocks * sizeof(local_max)));

    for (unsigned int i = 0; i < array_size - 1; ++i) {
        // copies column being used to gpu
        HANDLE_ERROR(cudaMemcpy(current_row_ptr, &i, sizeof(unsigned int), cudaMemcpyHostToDevice));

        // find the max in the column for the pivot
        columnMaxKernelDim(numblocks, numthreads, array_size);
        sdata_size = numthreads * 2;
        
        //        printf("SData Size: %d\n", sdata_size);
        //        printf("Numblocks: %d\n", numblocks);
        //        printf("Numthreads: %d\n", numthreads);
        //used in testing 
        
        //calls kernal finds the max
        columnMax << < numblocks, numthreads, sdata_size * sizeof(double) >> > (A, array_size_ptr, current_row_ptr, max_data_ptr);

        // allows the gpu to finish
        cudaDeviceSynchronize();

        HANDLE_ERROR(cudaMemcpy(max_data, max_data_ptr, numblocks * sizeof(local_max), cudaMemcpyDeviceToHost));

        unsigned int max_column_index = max_data[0].index;
        double max_column_value = max_data[0].value;
        for (int j = 0; j < numblocks; ++j) {
            if (fabs(max_data[j].value) > fabs(max_column_value) && max_data[j].index != -1.0) {
                max_column_index = max_data[j].index;
                max_column_value = max_data[j].value;
            }
        }

        //printf("Max row in column %d: %d\n", i, max_column_index);
        //testing finds to see if column max is right
        // singular matrix check
        if (max_column_value == 0.0) {
            printf("The matrix is singular!\n");
            singular = true;
            break;
        }

        HANDLE_ERROR(cudaMemcpy(row_to_swap_ptr, &max_column_index, sizeof(unsigned int), cudaMemcpyHostToDevice));
        swapRowsKernelDim(numblocks, numthreads, array_size);
        swapRows << < numblocks, numthreads, numthreads * sizeof(double) >> > (A, array_size_ptr, current_row_ptr, row_to_swap_ptr);

        // lets gpu finish
        cudaDeviceSynchronize();

        swapRowsKernelDim(numblocks, numthreads, array_size);
        findCoefficients << < numblocks, numthreads, numthreads * sizeof(double) >> > (A, array_size_ptr, current_row_ptr);

        // lets gpu finish
        cudaDeviceSynchronize();

        gaussianReductionKernelDim(numblocks, numthreads, array_size);
        dim3 grid(numblocks, numblocks);
        dim3 block(numthreads, numthreads);

        gaussianReduction << < grid, block >> > (A, array_size_ptr, current_row_ptr);

        // let gpu finish
        cudaDeviceSynchronize();


        HANDLE_ERROR(cudaMemcpy(array, A, array_area * sizeof(double), cudaMemcpyDeviceToHost));
    }

    // -----------------------------------------------------------------------------------------------------------------
    // End LU Decomp
    // -----------------------------------------------------------------------------------------------------------------

    if (!singular) {
        HANDLE_ERROR(cudaMemcpy(array, A, array_area * sizeof(double), cudaMemcpyDeviceToHost));

        if (toPrint == true)
        {
            printf("\nMatrix L\n");
            for (int i = 0; i < array_size; i++) {
                printf("Row %d: ", i);
                for (int j = 0; j < array_size; ++j) {
                    if (j < i) {
                        printf("%.2f\t", array[i * array_size + j]);
                    }
                    else if (j == i) {
                        printf("%.2f\t", 1.0);
                    }
                    else {
                        printf("%.2f\t", 0.0);
                    }
                }
                printf("\n");
            }

            printf("\nMatrix U\n");
            for (int i = 0; i < array_size; i++) {
                printf("Row %d: ", i);
                for (int j = 0; j < array_size; ++j) {
                    if (j >= i) {
                        printf("%.2f\t", array[i * array_size + j]);
                    }
                    else {
                        printf("%.2f\t", 0.0);
                    }
                }
                printf("\n");
            }
        }
    }


    // Calculate the runtime.
    runtime = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_usec - start.tv_usec) / 1000.0);
    printf("Processing Time: %4.4f milliseconds\n", runtime);

    // free cuda memory
    cudaFree(array_size_ptr);
    cudaFree(current_row_ptr);
    cudaFree(A);
    cudaFree(row_to_swap_ptr);
    cudaFree(max_data_ptr);

    // Free the allocated array.
    //free( array );

    return 0;
}
