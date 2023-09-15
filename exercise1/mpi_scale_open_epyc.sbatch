#!/bin/bash
#SBATCH -J mpi_scale_open_epyc
#SBATCH --get-user-env
#SBATCH --partition=EPYC
#SBATCH --nodes=2
#SBATCH -o mpi_scale_open_epyc.out
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load openMPI/4.1.5/gnu/12.2.1

make

export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=1

./main.x -i -k 10000 -f playground_thin.pgm

for i in {1..256}
do
	echo "using $i mpi processes on epyc nodes"
	mpirun -np $i --map-by core ./main.x -r -f playground_thin.pgm -n 10 -e 1 -s 0
	echo " "
	echo " "

done

echo "done"

