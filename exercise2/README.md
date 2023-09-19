# EXERCISE 2

The Exercise 2 consists in the comparing the MLK and OpenBLAS math libraries on the level 3 BLAS function called gemm.


## Files

In this folder you can find:

* `gemm.c`: the original gemm function provided.
* `my_gemm.c`: a modified version of gemm.c, useful for an easier storing of the results
* `Makefile`: used for compiling the necessary codes
* `compile.sh`: slurm script used for compiling the codes
* `close.sh`: slurm script used for measuring scalability over the size of the matrix, at 12 cores with the policy `OMP_PLACES=close`, on the THIN nodes, using MLK and OpenBLAS on both single and double precision. The size of the matrix is increased from 2000x2000 to 20000x20000 with steps of size 2000.
* `spread.sh`:slurm script used for measuring scalability over the size of the matrix, at 12 cores with the policy `OMP_PLACES=spread`, on the THIN nodes, using MLK and OpenBLAS on both single and double precision. The size of the matrix is increased from 2000x2000 to 20000x20000 with steps of size 2000.
* `size.sh`: slurm script used for measuring the scalability over cores, at size 12000, on the THIN nodes, using MLK and OpenBlas on both single and double precision. The number of cores is increased from 1 to 12.
* `results`: folder containing the results(in terms of time elapsed and GFlops) of the exercises. There is one .csv file for each combination of each exercise, and they contain the data used for the figures provided in the report.
