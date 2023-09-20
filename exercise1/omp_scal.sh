#!/bin/bash
#SBATCH -J omp_scal
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=2
#SBATCH -o omp_scal.out
#SBATCH --exclusive
#SBATCH --time=02:00:00
 
module load intelMPI/2021.7.1


make clean

make

export OMP_PLACES=cores
export OMP_PROC_BIND=close
export I_MPI_PIN_DOMAIN=socket

./main.x -i -k 10000 -f playground.pgm

echo size,threads,time > timings/omp_timings.csv

for i in {1..12}
do
	export OMP_NUM_THREADS=$i
        for j in {1..5}
        do
                echo -n 10000,$i>>timings/omp_timings.csv
        	mpirun -np 4 ./main.x -r -f playground.pgm -n 10 -e 1 -s 2 >>timings/omp_timings.csv
        done
done

echo "done"

