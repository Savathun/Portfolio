#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <time.h>
#include <mpi.h>
using namespace std;
#define MASTER 0 
bool GetUserInput(int argc, char *argv[],int& n,bool& isPrint)
{
	if(argc < 2) 
	{
		cout << "Arguments:<X> [<Y>]" << endl;
		cout << "X : Matrix size [X x X]" << endl;
		cout << "Y = 1: print the input/output matrices if X < 10" << endl;
		cout << "Y <> 1 or missing: does not print the input/output matrices" << endl;
		return false;
	}
	else 
	{
		//get matrix size
		n = atoi(argv[1]);
		if (n <=0) 
		{
			cout << "Matrix size must be larger than 0" <<endl;
			return false;
		}
		//is print the input/output matrix
		if (argc >=3) isPrint = (atoi(argv[2])==1 && n <=9)?1:0;
		else isPrint = 0;
	}
	return true;
}
float *InitializeMatrix(size_t n)
{
   int i, j, tot = n * n;
   float *M = (float*)malloc(sizeof(float) * tot);
   for (i = 0; i < tot; i++) {
      M[i] = (float)(rand()%10 + 1);
   }
   return M;
}

void PrintMatrix(float *a, int n) 
{
   int i, j;
   for (i = 0; i < n*n; i++) {
      printf("% *.*f\t", 4, 2, a[i]);
      if ((i + 1) % n == 0) {
         cout << endl;
      }
   }
		
}
/*
 * forw_elim - forward Gauss elimination
 *
 * @origin row pointer by reference
 * @master_row row in which lays diagonal
 */
void forw_elim(float **origin, float *master_row, size_t dim)
{
   if (**origin == 0)
      return;

   float k = **origin / master_row[0];

   int i;
   for (i = 1; i < dim; i++) {
      (*origin)[i] = (*origin)[i] - k * master_row[i];
   }
   **origin = k;
}

/*
 * U_print - dumb U matrix print function
 */
void U_print (float *M, int dim)
{
   int i, j;
   float z = 0;
   for (i = 0; i < dim; i++) {
      for (j = 0; j < dim; j++) {
         if (j >= i) {
            printf("% *.*f\t", 4, 2, M[i * dim + j]);
         } else {
            printf("% *.*f\t", 4, 2, z);
         }
      }
      cout << endl;
   }
}

/*
 * L_print - dumb L matrix print function
 */
void L_print (float *M, int dim)
{
   int i, j;
   float z = 0, u = 1;
   for (i = 0; i < dim; i++) {
      for (j = 0; j < dim; j++) {
         if (j > i) {
            printf("% *.*f\t", 4, 2, z);
         } else if (i == j) {
            printf("% *.*f\t", 4, 2, u);
         } else {
            printf("% *.*f\t", 4, 2, M[i * dim + j]);
         }
      }
      cout << endl;
   }
}
int main(int argc, char *argv[])
{
		int	numtasks,              /* number of tasks in partition */
		taskid,                /* a task identifier */
		i, j, n=0, rc;           /* misc */
		bool isPrintMatrix;
	double runtime;
	if (GetUserInput(argc, argv,n,isPrintMatrix)==false) return 1;
   srand(1);
   float *a = InitializeMatrix(n);
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
   MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
   if (taskid == MASTER && n < 10) {
		cout << "A Matrix: " << endl;
		PrintMatrix(a, n);
   }

   int diag_ref = 0;
   runtime = MPI_Wtime();

   for (i = 0; i < n - 1; i++, diag_ref++) {
	   
      float *diag_row = &a[diag_ref * n + diag_ref];
	  
      for (j = diag_ref + 1; j < n; j++) {
         if (j % numtasks == taskid) {
            float *save = &a[j * n + diag_ref];
            if(save != 0) forw_elim(&save, diag_row, n - diag_ref);
         }
      }
      for (j = diag_ref + 1; j < n; j++) {
         float *save = &a[j * n + diag_ref];
         MPI_Bcast(save, n - diag_ref, MPI_FLOAT, j % numtasks, MPI_COMM_WORLD);
      }

   }

   runtime = MPI_Wtime() - runtime; 

   if (taskid == MASTER && n < 10) {
      printf("\n[L]\n");
      L_print(a, n);
      printf("\n[U]\n");
      U_print(a, n);
   }
   if (taskid == MASTER) 		cout<< "LU Decomposition runs in "	<< setiosflags(ios::fixed) 
											<< setprecision(2)  
											<< runtime << " seconds \n";
   free(a);


   MPI_Finalize();
   return 0;
}
