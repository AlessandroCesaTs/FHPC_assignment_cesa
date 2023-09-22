#!/bin/bash
#SBATCH --job-name="size_close"
#SBATCH -o size_close.out
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH --cpus-per-task=24
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load mkl/2022.2.1
module load openBLAS/0.3.23-omp


export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=12

echo m,n,k,elapsed,gflops > results/sgemm_mlk_size_close.csv
echo m,n,k,elapsed,gflops > results/dgemm_mlk_size_close.csv
echo m,n,k,elapsed,gflops > results/sgemm_oblas_size_close.csv
echo m,n,k,elapsed,gflops > results/dgemm_oblas_size_close.csv

for i in {2000..20000..2000}
do
	for j in {1..5}
	do
		./sgemm_mkl.x $i $i $i >> results/sgemm_mlk_size_close.csv
		./dgemm_mkl.x $i $i $i >> results/dgemm_mlk_size_close.csv
		./sgemm_oblas.x $i $i $i >> results/sgemm_oblas_size_close.csv
		./dgemm_oblas.x $i $i $i >> results/dgemm_oblas_size_close.csv
	done
done

echo "done"
