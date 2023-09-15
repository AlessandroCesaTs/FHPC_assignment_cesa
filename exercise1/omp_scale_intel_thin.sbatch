#!/bin/bash
#SBATCH -J omp_scale_intel_thin
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH -o omp_scale_intel_thin.out
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load openMPI/4.1.5/gnu/12.2.1

make

module unload openMPI/4.1.5/gnu/12.2.1

module load intelMPI/2021.7.1

export OMP_PLACES=cores
export OMP_PROC_BIND=close
export I_MPI_PIN_DOMAIN=socket

./main.x -i -k 10000 -f playground_thin.pgm

for i in {1..12}
do
	export OMP_NUM_THREADS=$i
	echo "using $i threads"
	mpirun -n 1 ./main.x -r -f playground_thin.pgm -n 100 -e 1 -s 25
	echo " "
	echo " "

done

echo "done"

