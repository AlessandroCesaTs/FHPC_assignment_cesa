#!/bin/bash
#SBATCH -J omp_scale_intel_thin
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH -o omp_scale_intel_thin.out
#SBATCH --exclusive
#SBATCH --time=02:00:00
 
module load intelMPI/2021.7.1


make clean

make

export OMP_PLACES=cores
export OMP_PROC_BIND=close
export I_MPI_PIN_DOMAIN=socket

./main.x -i -k 10000 -f playground.pgm

for i in {1..12}
do
	export OMP_NUM_THREADS=$i
	echo "using $i threads"
	mpirun -n 2 ./main.x -r -f playground.pgm -n 10 -e 1 -s 2
	echo " "
	echo " "

done

echo "done"

