#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>

#include"initialize.h"
#include"image_handling.h"

void initialize(int size,char* name){
	/*function for intializing a square grid with rando 0s and 1s and
	 * write it in a pgm image
	 *size: grid will be size*size
	 *name: name of the file
	 */
    char *grid=(char *)malloc(size*size * sizeof(char) ); //initialize grid
   srand(time(NULL) );
#pragma omp parallel shared(grid,size,name) 
   {
#pragma omp for schedule(static)
    for (int i=0;i<size*size;i++){ //fill grid with random 0s and 1s
        grid[i]=(char)(rand()%2);
     }
    write_pgm_image((void *) grid, size, name); //write image to file
   }
    free(grid);
    return;
}

