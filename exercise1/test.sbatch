#!/bin/bash
#SBATCH -J myjob
#SBATCH -p EPYC --nodes 1 
#SBATCH -o test.out

echo "Doing 10 iterations on 100x100 grid, producing snapshot every 5 iterations"

./main.x -i -k 100 -f test_playground.pgm

./main.x -r -f test_playground.pgm -n 10 -e 1 -s 5

make clean

echo "done"

