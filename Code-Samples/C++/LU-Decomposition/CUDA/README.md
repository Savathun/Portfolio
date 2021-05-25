# Running the code

Compile: `ssh node18 nvcc <your_path>/CUDA/cuda_lud.cu -o <your_path>/CUDA/cuda_lud`

Replace <your_path> with the path to the CUDA directory. 

Run: `sbatch cuda_lud_slurm.sh <matrix_size> <seed_value> <print?>`

Replace <matrix_size> with matrix size, <seed_value> with the value of the random seed, and <print?> with a 0 or 1 depending if you want to print(will only print if n < 10).



