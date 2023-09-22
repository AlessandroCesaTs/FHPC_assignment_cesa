#!/bin/bash
#SBATCH --no-requeue
#SBATCH -J omp_single
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH -o omp_single.out
#SBATCH --exclusive
#SBATCH --time=02:00:00
 
module load intelMPI/2021.7.1


make clean

make



export OMP_PLACES=cores
export OMP_PROC_BIND=close
export I_MPI_PIN_DOMAIN=socket


for size in 10000 15000 30000
do
        ./main.x -i -k $size -f playground.pgm
        echo size,threads,time>timings/omp_single_$size.csv
        for i in {1..12}
        do
		export OMP_NUM_THREADS=$i

                for j in {1..5}
                do
                        echo -n $size,$i>>timings/omp_single_$size.csv
                        mpirun -n 1 ./main.x -r -f playground.pgm -n 10 -e 1 -s 0 >> timings/omp_single_$size.csv
                done
        done

done


echo "done"

