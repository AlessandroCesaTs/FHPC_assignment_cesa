#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>

#include"initialize.h"
#include"image_handling.h"

void initialize(int size,char* name){
    char *grid=(char *)malloc(size*size * sizeof(char) );
   srand(time(NULL) );
#pragma omp parallel shared(grid,size,name) 
   {
#pragma omp for schedule(static)
    for (int i=0;i<size*size;i++){
        grid[i]=(char)(rand()%2);
     }
    write_pgm_image((void *) grid, size, name);
   }
    free(grid);
    return;
}

