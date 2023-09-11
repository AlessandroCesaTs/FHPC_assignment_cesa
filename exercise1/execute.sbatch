#!/bin/bash
#SBATCH -J myjob
#SBATCH -p EPYC --nodes 1 -n 64 
#SBATCH -o execute.out

export OMP_PLACES=threads
export OMP_PROC_BIND=close

./main.x

echo "executed"
