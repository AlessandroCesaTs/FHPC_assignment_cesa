#!/bin/bash
#SBATCH -J myjob
#SBATCH -p EPYC --nodes 1 
#SBATCH -o execute.out

export OMP_PLACES=threads
export OMP_PROC_BIND=close

echo "Start executing"
echo " "
for i in {1..32}
do
	export OMP_NUM_THREADS=$i
	./main.x -i -k 10000 -f game_of_life.pgm
	./main.x -r
	echo " "
done

echo "Done"

