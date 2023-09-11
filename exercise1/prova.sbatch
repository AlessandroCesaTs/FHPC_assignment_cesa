#!/bin/bash
#SBATCH -J myjob
#SBATCH -p EPYC --nodes 1 -n 32 
#SBATCH -o prova.out

export OMP_PLACES=threads
export OMP_PROC_BIND=close


./prova.x

echo "xx"
