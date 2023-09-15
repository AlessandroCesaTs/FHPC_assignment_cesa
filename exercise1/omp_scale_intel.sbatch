#!/bin/bash
#SBATCH -J omp_scale_intel
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH -o omp_scale_intel.out

module load intelMPI/2021.7.1

export OMP_PLACES=cores
export OMP_PROC_BIND=close

export I_MPI_PIN_DOMAIN=socket

./main.x -i -k 1000 -f test_playground.pgm

for i in {1..12}
do
	export OMP_NUM_THREADS=$i
	echo "using $i threads"
	echo " "
	mpirun -n 2 ./main.x -r -f test_playground.pgm -n 1000 -e 1 -s 250
	echo " "
	echo " "

done


echo "done"

