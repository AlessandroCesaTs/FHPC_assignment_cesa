#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>

#include"initialize.h"
#include"image_handling.h"

void initialize(int size,char* name){
	/*function for intializing a square grid with random 0s and 1s and
	 * write it in a pgm image
	 *size: grid will be size*size
	 *name: name of the file
	 */

   char *grid=(char *)malloc(size*size * sizeof(char) ); //initialize grid
       int myid=5;
       unsigned int myseed = myid*myid+myid+1;
       char random;
       for (int i=0;i<size*size;i++){ //fill grid with random 0s and 1s
             random=(char)(rand_r(&myseed)%100<10? 1:0);
             grid[i]=random;
        } 
    
    write_pgm_image((void *) grid, size, name); //write image to file
    free(grid);
    return;
}


void initialize_test(char* name){
        /*function for intializing a predefined 4x4 grid in order to do a test for validation and
         * write it in a pgm image
         *size: grid will be size*size
         *name: name of the file
         */
   int size=4;
   char *grid=(char *)malloc(size*size * sizeof(char) ); //initialize grid
       for (int i=0;i<size*size;i++){ //fill grid with random 0s and 1s
             grid[i]=(char)0;
        }
   grid[5]=1;
   grid[15]=1;

    write_pgm_image((void *) grid, size, name); //write image to file
    free(grid);
    return;
}

