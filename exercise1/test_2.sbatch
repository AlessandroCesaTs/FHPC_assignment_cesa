#!/bin/bash
#SBATCH -J test_2
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=2 
#SBATCH -o test_2.out

module load intelMPI/2021.7.1


echo "Doing 100 static iterations on 1000x1000 grid, producing snapshot every 25 iterations;using 2 THIN nodes"

./main.x -i -k 1000 -f test_playground.pgm
mpirun -np 2 ./main.x -r -f test_playground.pgm -n 4 -e 1 -s 2

echo "done"

