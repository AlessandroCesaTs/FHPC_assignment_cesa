#ifndef RUN_H
#define RUN_H

void run(char* fname,int e,int n,int s);
void run_ordered(char* grid,int xsize,int n, int s);
void run_episode_ordered(char* grid,int size);
void run_static(char* grid,int size,int n,int s);
void run_episode_static(char* grid,char* eval,int size,int rank,int* lengths,int* starts);
char evaluate_cell(char* grid,int size,int i);

#endif

