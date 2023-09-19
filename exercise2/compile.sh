#!/bin/bash
#SBATCH --job-name="compile"
#SBATCH -o compile.out
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH --cpus-per-task=24
#SBATCH --exclusive
#SBATCH --time=00:05:00

make clean

module load mkl/2022.2.1
module load openBLAS/0.3.23-omp

make

echo "done"
