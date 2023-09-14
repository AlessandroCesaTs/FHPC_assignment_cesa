#ifndef RUN_H
#define RUN_H

void run(char* fname,int e,int n,int s);
void upgrade_cell(char** grid,int size,int i,int j);
void run_episode_ordered(char** grid,int size);
void run_episode_static(char** grid,int size,int world_rank,int world_size);
char evaluate_cell(char** grid,int size,int i,int j);

#endif

