#!/bin/bash
#SBATCH -J omp_scale_open_thin
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH -o omp_scale_open_thin.out
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load openMPI/4.1.5/gnu/12.2.1

make

export OMP_PLACES=cores
export OMP_PROC_BIND=close

./main.x -i -k 10000 -f playground_thin.pgm

for i in {1..12}
do
	export OMP_NUM_THREADS=$i
	echo "using $i threads"
	mpirun -np 2 --map-by socket ./main.x -r -f playground_thin.pgm -n 100 -e 1 -s 25
	echo " "
	echo " "

done

echo "done"

