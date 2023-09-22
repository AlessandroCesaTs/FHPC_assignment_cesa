#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>
#include<mpi.h> 
#include<string.h>
#include "image_handling.h"
#include "run.h"

#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec +     \
                  (double)ts.tv_nsec * 1e-9)

#define ORDERED 0

//functions for running conway's game of life


char evaluate_cell(char* grid,int size,int i){
        /*function for evaluating if a cell will be dead or alive
         * grid: pointer to the grid
         * size: grid is size*size
         * i: position of the cell
         */
    const int row=i/size; //get row and column of cell
    const int col=i%size;

    int up_row = (row - 1 + size) % size; //compute coordinates of neighbors
    int down_row = (row + 1) % size;
    int left_col = (col - 1 + size) % size;
    int right_col = (col + 1) % size;

    int neighborhood;
    //compute how many cells in neighborhood are alive
    neighborhood=grid[up_row*size+left_col]+grid[up_row*size+col]+grid[up_row*size+right_col]
	        +grid[row*size+left_col]+grid[row*size+right_col]
		+grid[down_row*size+left_col]+grid[down_row*size+col]+grid[down_row*size+right_col];
    //1=alive,0=dead
    if (neighborhood==2 || neighborhood==3){
            return (char)1;
        }else{
            return (char)0;
        }
}



void run_episode_ordered(char* grid,int size){
	/*function for running episode in ordered mode
	 * grid: pointer to the grid
         * size: grid is size*size
	 */
	//update one cell after the other
    //char* g=*grid;
    for (int i=0;i<size*size;i++){
	 grid[i]=evaluate_cell(grid,size,i);
    }
    return;
}

void run_episode_static(char* grid,char* eval,int size,int rank,int* lengths,int* starts){
       /*function for running episode in static
         * grid: pointer to the grid
	 * eval: array where the next states will be stored
         * size: grid is size*size
	 * rank: MPI rank
	 * lengths: lengths of the parts of grid that each task will work on
	 * starts: starting points of the parts of the grid where each tsk will work on
         */
        //first evaluate all grid and then change cells
	
     MPI_Barrier(MPI_COMM_WORLD);
     #pragma omp parallel for schedule(static)  //parallilize with OpenMP
     for (int i=0;i<lengths[rank];i++){
            eval[i]=evaluate_cell(grid,size,i+starts[rank]); //fill eval with the new states of cells
      }

    MPI_Allgatherv((const void*)eval,lengths[rank],MPI_CHAR,(void*)grid,lengths,starts,MPI_CHAR,MPI_COMM_WORLD); //each process sends its new 															states to the others, which 														      copy them in the comlete gri                                                                                                                  d 

    return;
}

void run_static(char* grid,int size,int n,int s){
    /*function for running in static
         * grid: pointer to the grid
         * size: grid is size*size
         * n: number of episodes
         * s :every how many episodes print the grid
         */

    struct timespec ts; //start measuring time
    double t_start=CPU_TIME;
    
    MPI_Init(NULL,NULL); 
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* lengths=(int*)malloc(world_size*sizeof(int)); //initialize array for lengths of the parts of grid that each task will work on
    int* starts=(int*)malloc(world_size*sizeof(int));  //initialize array for starting points on th grid  of the parts that each task will wor							       k on
    
    int rest=(size*size)%world_size; //remainder of division of size of grid for number of taks

    for (int i=0; i<world_size;i++){  //compute lengths and starts by dividing and distributing the remainder
    	lengths[i]=(size*size)/world_size;
	starts[i]=i*lengths[i];
	if (i<rest){
       		lengths[i]+=1;
        	starts[i]+=i;
   	 }else{
        	starts[i]+=rest;
    	 }

    }

    char* eval = (char*)malloc(lengths[rank]*sizeof(char )); //array that will store the next state for the part of the grid each rank will wo                                                               rk on 

    for (int t=0;t<n;t++){ //for each episode, run and rank 0 prints the image if necessary
	run_episode_static(grid,eval,size,rank,lengths,starts);
        if(rank==0&&((s!=0&&t%s==0)||(t==n-1))){
            char new_image_name[25];
            snprintf(new_image_name, sizeof(new_image_name), "snapshot_%d.pgm", t);
            write_pgm_image((void*)grid, size, new_image_name);
        }
    }

    free(eval);
    free(lengths);
    free(starts);

    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0){                 //print time
    	printf(",%g\n", CPU_TIME-t_start);
    }

    MPI_Finalize();

    return;

}


void run_ordered(char* grid,int xsize,int n, int s){
     /*function for running in ordered
         * grid: pointer to the grid
         * size: grid is size*size
         * n: number of episodes
         * s :every how many episodes print the grid
         */
    struct timespec ts;
    double t_start=CPU_TIME; //start taking time
    for(int t=0;t<n;t++){ //at each episode run and if necessary print 
        run_episode_ordered(grid,xsize);
        if((s!=0&&t%s==0)||(t==n-1)){
            char new_image_name[25];
            snprintf(new_image_name, sizeof(new_image_name), "snapshot_%d.pgm", t);
            write_pgm_image((void*)grid, xsize, new_image_name);
        }
    }
    printf(",%g\n", CPU_TIME-t_start); //print time
    return;

}

void run(char* fname,int e,int n,int s){
   /*function for running Conway's Game of Life
    * fname: name of file from where to read the grid
    * e: run ordered or static
    * n: number of episodes
    * s :every how many episodes print the grid
    */	
	
    char* grid;
    int maxval;
    int xsize;
    int ysize;
    read_pgm_image((void**)&grid,&maxval,&xsize,&ysize,fname);

    if (e==ORDERED){
            run_ordered(grid,xsize,n,s);
    }
    else{
            run_static(grid,xsize,n,s);
    }
   
    return;
}

