#!/bin/bash
#SBATCH -J weak_mpi_scal_cores
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH -o weak_mpi_scal_cores.out
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load intelMPI/2021.7.1

make clean

make

export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=1
export I_MPI_PIN_DOMAIN=core

size=(0 10000 14142 17321 20001 22360 24495 2646 28458 30000 31623 33166 34641 36056 37417 38730 40000 41231 42426 43589 44721 45825 46904 47958 48990)

echo size,cores,time > timings/weak_mpi_cores_timings.csv 

for i in {1..24}
do
	./main.x -i -k ${size[i]} -f playground.pgm
	
	for j in {1..5}
	do
		echo -n ${size[i]},$i >> timings/weak_mpi_cores_timings.csv
		mpirun -np $i ./main.x -r -f playground.pgm -n 10 -e 1 -s 2 >> timings/weak_mpi_cores_timings.csv
	done
done 


echo "done"

