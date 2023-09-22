#!/bin/bash
#SBATCH --job-name="size_spread"
#SBATCH -o size_spread.out
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH --cpus-per-task=24
#SBATCH --exclusive
#SBATCH --time=01:00:00


module load mkl/2022.2.1
module load openBLAS/0.3.23-omp

export OMP_PLACES=cores
export OMP_PROC_BIND=spread
export OMP_NUM_THREADS=12

echo m,n,k,elapsed,gflops > results/sgemm_mlk_size_spread.csv
echo m,n,k,elapsed,gflops > results/dgemm_mlk_size_spread.csv
echo m,n,k,elapsed,gflops > results/sgemm_oblas_size_spread.csv
echo m,n,k,elapsed,gflops > results/dgemm_oblas_size_spread.csv

for i in {2000..20000..2000}
do
	for j in {1..5}
	do
		./sgemm_mkl.x $i $i $i >> results/sgemm_mlk_size_spread.csv
		./dgemm_mkl.x $i $i $i >> results/dgemm_mlk_size_spread.csv
		./sgemm_oblas.x $i $i $i >> results/sgemm_oblas_size_spread.csv
		./dgemm_oblas.x $i $i $i >> results/dgemm_oblas_size_spread.csv
	done
done

echo "done"
