#!/bin/bash
#SBATCH --no-requeue
#SBATCH -J strong_mpi
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=2
#SBATCH -o strong_mpi.out
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load intelMPI/2021.7.1

make clean

make

export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=1
export I_MPI_PIN_DOMAIN=1
export I_MPI_PIN_ORDER=compact



for size in 10000 15000 25000
do
	./main.x -i -k $size -f playground.pgm
	echo size,tasks,time>timings/strong_mpi_$size.csv
	for i in {1..48}
	do
		for j in {1..5}
		do
			echo -n $size,$i>>timings/strong_mpi_$size.csv
			mpirun -n $i ./main.x -r -f playground.pgm -n 10 -e 1 -s 0 >> timings/strong_mpi_$size.csv
		done
	done
done

echo "done"

