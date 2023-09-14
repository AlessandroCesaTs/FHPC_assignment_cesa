#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>
#include<mpi.h> 

#include"run.h"
#include "image_handling.h"

#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec +     \
                  (double)ts.tv_nsec * 1e-9)

#define ORDERED 0

//functions for running conway's game of life

void run(char* fname,int e,int n,int s){
	char* grid;
        int maxval;
        int xsize;
	int ysize;
	read_pgm_image((void**)&grid,&maxval,&xsize,&ysize,fname);
	MPI_Init(NULL,NULL);
        for (int t=0;t<n;t++){
		 if (e==ORDERED){
                	 run_episode_ordered(&grid,xsize);
		 }else{
			 run_episode_static(&grid,xsize);
		 }
		 if((s!=0&&t%s==0)||t==n-1){
                           char new_image_name[25];
    			   snprintf(new_image_name, sizeof(new_image_name), "snapshot_%d.pgm", t);
                             write_pgm_image((void*)grid, xsize, new_image_name);
                  }
               }
	MPI_Finalize();
}

char evaluate_cell(char** grid,int size,int i,int j){
	/*function for evaluating if a cell will be dead or alive
	 * grid: pointer to the grid
	 * size: grid is size*size
	 * i,j: coordinates of the cell
	 */
    int u_i,d_i,l_j,r_j; //coordinates of neighbors u=up, d=down, l=left,r=right
    int neighborhood;
    char* g=*grid; //de-reference
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
    neighborhood=(int)g[u_i*size+l_j]+(int)g[u_i*size+j]+
	    (int)g[u_i*size+r_j]+(int)g[i*size+l_j]+      
	    (int)g[i*size+r_j]+(int)g[d_i*size+l_j]+(int)g[d_i*size+j]+(int)g[d_i*size+r_j];
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
    g[i*size+j]=evaluate_cell(grid,size,i,j);
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

void run_episode_static(char** grid,int size){
	/*function for running episode in ordered mode
         * grid: pointer to the grid
         * size: grid is size*size
         */
	//first evaluate all grid and then change cells
	//grid for evaluations

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int my_size=(size*size)/world_size;
    int my_start=world_rank*my_size;
    if(world_rank==world_size-1){
    	my_size+= (size*size)%world_size;
    }

    int* recvcounts=NULL;
    int* displacements=NULL;
    if (world_rank==0){
    	recvcounts=(int*)malloc(world_size*sizeof(int));
	displacements=(int*)malloc(world_size*sizeof(int));
    }

    MPI_Gather((const void*)&my_size,1,MPI_INT,(void*)recvcounts,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Gather((const void*)&my_start,1,MPI_INT,(void*)displacements,1,MPI_INT,0,MPI_COMM_WORLD);
    /* if (world_rank == 0) {
        printf("recvcounts: ");
        for (int i = 0; i < world_size; i++) {
            printf("%d ", recvcounts[i]);
        }
        printf("\n");
	printf("displacements: ");
        for (int i = 0; i < world_size; i++) {
            printf("%d ", recvcounts[i]);
        }
        printf("\n");

    }
*/
    char *eval = (char *)malloc(my_size*sizeof(char ));
     if (eval == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        MPI_Abort(MPI_COMM_WORLD, 1); // Abort MPI on error
    }
    for (int i=0;i<my_size;i++){
            eval[i]=evaluate_cell(grid,size,(i+my_start)/size,(i+my_start)%size);//fill with all evaluations
    }

    MPI_Gatherv((const void*)eval,my_size,MPI_CHAR,(void*)*grid,recvcounts,displacements,MPI_CHAR,0,MPI_COMM_WORLD);

    if (world_rank==0){
    	free(recvcounts);
	free(displacements);
    }
    free(eval);

    return;
}
