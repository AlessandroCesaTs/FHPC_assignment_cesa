#!/bin/bash
#SBATCH -J test_2
#SBATCH --get-user-env
#SBATCH -p THIN --nodes 2 
#SBATCH -o test_2.out

echo "Doing 100 static iterations on 1000x1000 grid, producing snapshot every 25 iterations;using 2 THIN nodes"

mpirun -np 2 ./main.x -i -k 1000 -f test_playground.pgm
mpirun -np 1 ./main.x -r -f test_playground.pgm -n 100 -e 1 -s 25

echo "done"

