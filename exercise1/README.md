# EXERCISE 1


The Exercise 1 is an implementation of Conway's Game of Life, using openMP and MPI.


## Compilation

In order to compile the code, you should load IntelMPI on your system. On the ORFEO cluster you can run the command 'module load intelMPI'

To compile, you can use the ``Makefile``, and run it on a THIN node using `srun -n1 -N1 -p THIN make`.
This will produce the ecxecutable 'make.x'

## Running

In order to run the code, the suggested approach is to use a **slurm** script, and run it with the command `sbatch`. 

Inside the slurm script,the  executable will have to be run with
`mpirun -np <number of processes>./main.x <-i/-r> -k <playground size> -e <0/1> -f<name of file> -n <number of steps> -s <every how many steps print playground, 0 only at the end> ` .

If you execute with `-i`, it will initialize a playground of the given size and print it in a file of the given name; in this case you won't need to give the arguments `-e`, `-n` and `-f`. The size needs to be at least 100, otherwise an error will be outputted.

If you execute  with `-r` it will read a playground from the file of the given name and it will run for the given number of steps,in ordered evolution if you give `-e 0` and in statice evolution if you give `-e 1`; it will print the result with the asked frequency, you won'tneed to g
ive the argument `-k`.


Otherwise, you can allocate the needed resources with the command `salloc -N<number of nodes> -n<number of tasks>`, and then run the executable as previously described.

##Files

In the present directory you can find:

* `Makefile`: used to compile the code
* `test.sh`: slurm script used for running a small test to validate my implementation of Conway's Game of Life
* `omp_scale.sh`: slurm script used for the OpenMP scaling exercise
* `weak_mpi_scale.sh`: slurm script used for the weak MPI scaling exercise
* `strong_mpi_scale.sh`: slurm script used for the strong MPI scaling exercise
* `source`: folder containing all the C source codes necessary for the exercise
* `headers`: folder containing all the header files necessary for compiling the source codes
* `objects`: folder used to store object files 
* `timings`: folder containing the timings of the exercises. There is one .csv file for each exercise, and they contain the data used for the figures provided in the report 
