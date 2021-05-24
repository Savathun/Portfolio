# Running the code

Compile: `g++ -O -fopenmp omp_lud.cpp -o omp_lud`

Run: `sbatch omp_lud_slurm.sh <matrix_size> <threads> <print?>`

Replace <matrix_size> with matrix size, <threads> with the number of threads, and <print?> with a 0 or 1 depending if you want to print(will only print if n < 10).

srand is set to 1, so you should get all the same values as we did if you keep it at 1.
