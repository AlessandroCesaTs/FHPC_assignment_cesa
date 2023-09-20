#!/bin/bash
#SBATCH -J weak_mpi_scal_socket
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=2
#SBATCH -o weak_mpi_scal_socket.out
#SBATCH --exclusive
#SBATCH --time=02:00:00


module load intelMPI/2021.7.1

make clean

make

export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=12
export I_MPI_PIN_DOMAIN=socket

size=(0 100000 141421 173205 200000)

echo size,sockets,time > timings/weak_mpi_socket_timings.csv 

for i in {1..4}
do
	./main.x -i -k ${size[i]} -f playground.pgm
	
	for j in {1..5}
	do
		echo -n ${size[i]},$i >> timings/weak_mpi_socket_timings.csv
		mpirun -np $i ./main.x -r -f playground.pgm -n 10 -e 1 -s 2 >> timings/weak_mpi_socket_timings.csv
	done
done 


echo "done"

