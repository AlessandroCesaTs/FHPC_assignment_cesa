#!/bin/bash
#SBATCH --job-name="cores_spread"
#SBATCH -o cores_spread.out
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH --cpus-per-task=24
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load mkl/2022.2.1
module load openBLAS/0.3.23-omp

export OMP_PLACES=cores
export OMP_PROC_BIND=spread

size=10000

echo cores,m,n,k,elapsed,gflops > results/sgemm_mlk_cores_spread.csv
echo cores,m,n,k,elapsed,gflops > results/dgemm_mlk_cores_spread.csv
echo cores,m,n,k,elapsed,gflops > results/sgemm_oblas_cores_spread.csv
echo cores,m,n,k,elapsed,gflops > results/dgemm_oblas_cores_spread.csv

for i in {1..12}
do
	export OMP_NUM_THREADS=$i
	for j in {1..5}
	do
		printf "$i," >> results/sgemm_mlk_cores_spread.csv
                printf "$i," >> results/dgemm_mlk_cores_spread.csv
                printf "$i," >> results/sgemm_oblas_cores_spread.csv
                printf "$i," >> results/dgemm_oblas_cores_spread.csv

		./sgemm_mkl.x $size $size $size >> results/sgemm_mlk_cores_spread.csv
		./dgemm_mkl.x $size $size $size >> results/dgemm_mlk_cores_spread.csv
		./sgemm_oblas.x $size $size $size >> results/sgemm_oblas_cores_spread.csv
		./dgemm_oblas.x $size $size $size >> results/dgemm_oblas_cores_spread.csv
	done
done

echo "done"
