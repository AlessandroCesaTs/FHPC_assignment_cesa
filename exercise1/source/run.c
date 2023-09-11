#include<stdio.h>
#include<stdlib.h>

#include"run.h"
#include "image_handling.h"


void upgrade_cell(char** grid,int size,int i,int j){
    char* g=*grid;
    printf("call evaluate cell \n");
    fflush(stdout);
    g[i*size+j]=evaluate_cell(grid,size,i,j);
}


void run_episode_ordered(char** grid,int size){
	printf("start running episode\n");
	fflush(stdout);
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
		printf("call upgrade_cell \n");
		fflush(stdout);
            upgrade_cell(grid,size,i,j);
        }
    }
}

void run_episode_static(char** grid,int size){
    char **eval = (char **)malloc(size * sizeof(char *));
    for (int i=0;i<size;i++){
        eval[i]=(char*)malloc(size * sizeof(char));
        for (int j=0;j<size;j++){
            eval[i][j]=evaluate_cell(grid,size,i,j);
        }
    }
char* g=*grid;
    for (int i=0;i<size;i++){
            for (int j=0;j<size;j++){
           g[i*size+j]=eval[i][j];
            }
    }
    free(eval);
}

char evaluate_cell(char** grid,int size,int i,int j){
    printf("evaluate cell called\n");
    fflush(stdout);
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
    printf("compute neighborhhod\n ");
    fflush(stdout);
    neighborhood=(int)g[u_i*size+l_j]+(int)g[u_i*size+j]+
	    (int)g[u_i*size+r_j]+(int)g[i*size+l_j]+
	    (int)g[i*size+r_j]+(int)g[d_i*size+l_j]+(int)g[d_i*size+j]+(int)g[d_i*size+r_j];
    printf("neighborhood computed \n");
    fflush(stdout);
 if (neighborhood==2 || neighborhood==3){
            return (char)1;
        }else{
            return (char)0;
        }
}

