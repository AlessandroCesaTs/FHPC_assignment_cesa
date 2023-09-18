# EXERCISE 1


The Exercise 1 is an implementation of Conway's Game of Life, using openMP and MPI.


## Compilation

In order to compile the code, you should load IntelMPI on your system. On the ORFEO cluster you can run the command 'module load intelMPI'

To compile, you can use the ''Makefile'', and run it on a THIN node using 'srun -n1 -N1 -p THIN make'.
This will produce the ecxecutable 'make.x'

## Running

In order to run the code, the suggested approach is to use a **slurm** script, and run it with the command 'sbatch'. 

Inside the slurm script,the  executable will have to be run with './main.x $lt$ -i/-r $gt$ -k $lt$ playground size $gt$ -e $lt$0/1$gt$ -f$lt$ name of file $gt$ -n $lt$ number of steps$gt$ -s $lt$  every how many steps print playground, 0 only at the end $gt$ ' .

If you execute with '-i', it will initialize a playground of the given size and print it in a file of the given name; in this case you won't need to give the arguments '-e', '-n' and '-f'. The size needs to be at least 100, otherwise an error will be outputted.

If you execute  with '-r' it will read a playground from the file of the given name and it will run for the given number of steps,in ordered evolution if you give '-e 0' and in statice evolution if you give '-e 1'; it will print the result with the asked frequency, you won'tneed to g
ive the argument '-k'.


Otherwise, you can allocate the needed resources with the command 'salloc -N&ltnumber of nodes&gt -n&ltnumber of tasks&gt', and then run the executable as previously described.

