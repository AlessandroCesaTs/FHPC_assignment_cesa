#!/bin/bash
#SBATCH --job-name="close"
#SBATCH -o close.out
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH --cpus-per-task=24
#SBATCH --exclusive
#SBATCH --time=01:00:00

make clean

module load mkl/2022.2.1
module load openBLAS/0.3.23

make


export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=12

echo m,n,k,elapsed,gflops > results/sgemm_mlk_cores_close.csv
echo m,n,k,elapsed,gflops > results/dgemm_mlk_cores_close.csv
echo m,n,k,elapsed,gflops > results/sgemm_oblas_cores_close.csv
echo m,n,k,elapsed,gflops > results/sgemm_oblas_cores_close.csv

for i in {2000..20000..2000}
do
	for j in {1..5}
	do
		./sgemm_mkl.x $i $i $i >> results/sgemm_mlk_cores_close.csv
		./dgemm_mkl.x $i $i $i >> results/dgemm_mlk_cores_close.csv
		./sgemm_oblas.x $i $i $i >> results/sgemm_oblas_cores_close.csv
		./dgemm_oblas.x $i $i $i >> results/dgemm_oblas_cores_close.csv
	done
done

echo "done"
