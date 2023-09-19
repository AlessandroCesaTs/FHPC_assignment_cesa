#!/bin/bash
#SBATCH --job-name="size"
#SBATCH -o size.out
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

size=10000

echo cores,m,n,k,elapsed,gflops > results/sgemm_mlk_size.csv
echo cores,m,n,k,elapsed,gflops > results/dgemm_mlk_size.csv
echo cores,cores,m,n,k,elapsed,gflops > results/sgemm_oblas_size.csv
echo cores,m,n,k,elapsed,gflops > results/dgemm_oblas_size.csv

for i in {1..12}
do
	export OMP_NUM_THREADS=$i
	for j in {1..5}
	do
		echo $i, >> results/sgemm_mlk_size.csv
                echo $i, >> results/dgemm_mlk_size.csv
                echo $i, >> results/sgemm_oblas_size.csv
                echo $i, >> results/dgemm_oblas_size.csv

		./sgemm_mkl.x $size $size $size >> results/sgemm_mlk_size.csv
		./dgemm_mkl.x $size $size $size >> results/dgemm_mlk_size.csv
		./sgemm_oblas.x $size $size $size >> results/sgemm_oblas_size.csv
		./dgemm_oblas.x $size $size $size >> results/dgemm_oblas_size.csv
	done
done

echo "done"
