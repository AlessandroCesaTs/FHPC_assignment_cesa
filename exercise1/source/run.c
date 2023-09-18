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


char evaluate_cell(char* grid,int size,int i,int j){
	/*function for evaluating if a cell will be dead or alive
	 * grid: pointer to the grid
	 * size: grid is size*size
	 * i,j: coordinates of the cell
	 */
    int u_i,d_i,l_j,r_j; //coordinates of neighbors u=up, d=down, l=left,r=right
    int neighborhood;
    if (i==0){ //compute neighbors, considering border contitions
        u_i=size-1; 
        d_i=i+1;
    }else if(i==size-1){
        u_i=i-1;
        d_i=0;
    }else{
        u_i=i-1;
        d_i=i+1;
    }

    if(j==0){
        l_j=size-1;
        r_j=j+1;
    }else if(j==size-1){
        l_j=j-1;
        r_j=0;
    }else{
        l_j=j-1;
        r_j=j+1;
    }
    //compute how many cells in neighborhood are alive
    neighborhood=grid[u_i*size+l_j]+grid[u_i*size+j]+
	    grid[u_i*size+r_j]+grid[i*size+l_j]+      
	    grid[i*size+r_j]+grid[d_i*size+l_j]+grid[d_i*size+j]+grid[d_i*size+r_j];
    //1=alive,0=dead
    if (neighborhood==2 || neighborhood==3){
            return (char)1;
        }else{
            return (char)0;
        }
}

void update_cell(char** grid,int size,int i,int j){
	/*function for updating a single cell
	 *grid: pointer to the grid
         * size: grid is size*size
         * i,j: coordinates of the cell
	 */
    char* g=*grid;
    g[i*size+j]=evaluate_cell(g,size,i,j);
    return;
}


void run_episode_ordered(char** grid,int size){
	/*function for running episode in ordered mode
	 * grid: pointer to the grid
         * size: grid is size*size
	 */
	//update one cell after the other
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            update_cell(grid,size,i,j);
        }
    }
    return;
}


void run_episode_static(char** grid,int size,int my_size,int my_start,int* recvcounts,int* displacements){
       /*function for running episode in ordered mode
         * grid: pointer to the grid
         * size: grid is size*size
         */
        //first evaluate all grid and then change cells
        //grid for evaluations
    char *eval = (char *)malloc(my_size*sizeof(char ));
     if (eval == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        MPI_Abort(MPI_COMM_WORLD, 1); // Abort MPI on error
    }
     #pragma omp parallel for schedule(static,size)
     for (int i=0;i<my_size;i++){
            eval[i]=evaluate_cell(*grid,size,(i+my_start)/size,(i+my_start)%size);
      }
    
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Allgatherv((const void*)eval,my_size,MPI_CHAR,(void*)*grid,recvcounts,displacements,MPI_CHAR,MPI_COMM_WORLD);
    free(eval);

    return;
}

void run_static(char** grid,int size,int n,int s){
    struct timespec ts;
    double t_start=CPU_TIME;
    int mpi_provided_thread_level;
	MPI_Init_thread(NULL,NULL, MPI_THREAD_FUNNELED,
	&mpi_provided_thread_level);
	if ( mpi_provided_thread_level < MPI_THREAD_FUNNELED ) {
		printf("a problem arise when asking for MPI_THREAD_FUNNELED level\n");
		MPI_Finalize();
		exit( 1 );
	}
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int my_size=(size*size)/world_size;
    int my_start=rank*my_size;
    if(rank==world_size-1){
    	my_size+= (size*size)%world_size;
    }

    int* recvcounts=(int*)malloc(world_size*sizeof(int));
    int* displacements=(int*)malloc(world_size*sizeof(int));

    MPI_Allgather((const void*)&my_size,1,MPI_INT,(void*)recvcounts,1,MPI_INT,MPI_COMM_WORLD);
    MPI_Allgather((const void*)&my_start,1,MPI_INT,(void*)displacements,1,MPI_INT,MPI_COMM_WORLD);

    //MPI_Barrier(MPI_COMM_WORLD);

    for (int t=0;t<n;t++){
        run_episode_static(grid,size,my_size,my_start,recvcounts,displacements);
        if(((s!=0&&t%s==0)||(t==n-1))&&rank==0){
            char new_image_name[25];
            snprintf(new_image_name, sizeof(new_image_name), "snapshot_%d.pgm", t);
            write_pgm_image((void*)*grid, size, new_image_name);
        }
    }
    if(rank==0){
    	printf("Process took %g \n", CPU_TIME-t_start);
    }

    free(recvcounts);
    free(displacements);

    MPI_Finalize();

    return;

}


void run_ordered(char** grid,int xsize,int n, int s){
    struct timespec ts;
    double t_start=CPU_TIME;
    for(int t=0;t<n;t++){
        run_episode_ordered(grid,xsize);
        if((s!=0&&t%s==0)||(t==n-1)){
            char new_image_name[25];
            snprintf(new_image_name, sizeof(new_image_name), "snapshot_%d.pgm", t);
            write_pgm_image((void*)*grid, xsize, new_image_name);
        }
    }
    printf("Process took %g \n", CPU_TIME-t_start);
    return;

}

void run(char* fname,int e,int n,int s){
	
    char* grid;
    int maxval;
    int xsize;
    int ysize;
    read_pgm_image((void**)&grid,&maxval,&xsize,&ysize,fname);

    if (e==ORDERED){
            run_ordered(&grid,xsize,n,s);
    }
    else{
            run_static(&grid,xsize,n,s);
    }
   
    return;
}

