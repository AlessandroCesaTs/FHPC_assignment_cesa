#include<stdio.h>
#include<stdlib.h>

#include"run.h"
#include "image_handling.h"

char evaluate_cell(char** grid,int size,int i,int j){
    int u_i,d_i,l_j,r_j;
    int neighborhood;
    char* g=*grid;
    if (i==0){
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
    neighborhood=(int)g[u_i*size+l_j]+(int)g[u_i*size+j]+
	    (int)g[u_i*size+r_j]+(int)g[i*size+l_j]+
	    (int)g[i*size+r_j]+(int)g[d_i*size+l_j]+(int)g[d_i*size+j]+(int)g[d_i*size+r_j];
 if (neighborhood==2 || neighborhood==3){
            return (char)1;
        }else{
            return (char)0;
        }
}

void update_cell(char** grid,int size,int i,int j){
    char* g=*grid;
    g[i*size+j]=evaluate_cell(grid,size,i,j);
}


void run_episode_ordered(char** grid,int size){
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            update_cell(grid,size,i,j);
        }
    }
}

void run_episode_static(char** grid,int size){
    char **eval = (char **)malloc(size * sizeof(char *));
    #pragma omp parallel for shared(eval,grid,size) schedule(static)
    for (int i=0;i<size;i++){
        eval[i]=(char*)malloc(size * sizeof(char));
        for (int j=0;j<size;j++){
            eval[i][j]=evaluate_cell(grid,size,i,j);
        }
    }
char* g=*grid;
#pragma omp parallel for shared(g,eval) schedule(static)
    for (int i=0;i<size;i++){
            for (int j=0;j<size;j++){
           g[i*size+j]=eval[i][j];
            }
    }
    free(eval);
}
