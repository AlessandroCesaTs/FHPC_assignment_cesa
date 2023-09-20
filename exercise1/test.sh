#!/bin/bash
#SBATCH -J test
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=1
#SBATCH -o test.out

#sbatch for validation test

module load intelMPI/2021.7.1

make clean

make

./main.x -i -k 4 -f playground_test.pgm

mpirun -n 3 ./main.x -r -f playground_test.pgm -n 2 -e 1 -s 1

echo "done"

