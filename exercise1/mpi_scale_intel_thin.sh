#!/bin/bash
#SBATCH -J mpi_scale_intel_thin
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=2
#SBATCH -o mpi_scale_intel_thin.out
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load intelMPI/2021.7.1

make clean

make

export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=1

./main.x -i -k 10000 -f playground.pgm

echo size,cores,time>timings/mpi_timings.csv

for i in {1..48}
do
	for j in {1..5}
	do
		echo -n 10000,$i>>timings/mpi_timings.csv
		mpirun -np $i --map-by core ./main.x -r -f playground.pgm -n 10 -e 1 -s 2 >> timings/mpi_timings.csv
	done
done

echo "done"

