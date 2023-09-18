#!/bin/bash
#SBATCH --job-name="test"
#SBATCH -o test.out
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

echo m,n,k,elapsed,gflops > test.csv

./sgemm_mkl.x 2000 2000 2000 >> test.csv

