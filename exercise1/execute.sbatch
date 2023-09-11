#!/bin/bash
#SBATCH -J myjob
#SBATCH -p EPYC --nodes 1 
#SBATCH -o execute.out

export OMP_PLACES=cores
export OMP_PROC_BIND=spread

for i in {1..32}
do
	export OMP_NUM_THREADS=$i
	./main.x
done



