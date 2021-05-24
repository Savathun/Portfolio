# Running the code

Compile: `mpicxx mpi_lud.cpp -o mpi_lud`

Run: `sbatch -F hosts mpi_lud_slurm.sh <matrix_size> <print?>`

Replace <matrix_size> with matrix size, and <print?> with a 0 or 1 depending if you want to print(will only print if n < 10).

Alternately: `mpirun -print-rank-map -check-mpi -n <tasks> -ppn <process-per-node> -f hosts ./mpi_lud <matrix_size> <print?>`

To change the number of tasks. 

srand is set to 1, so you should get all the same values as we did if you keep it at 1.
