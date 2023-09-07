#include <stdio.h>
#include <stdlib.h>

void print_grid(int** grid){
    int size=sizeof(grid[0]);
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            //printf("(%d,%d)=%d \n",i,j,grid[i][j]);
            printf(" %d ",grid[i][j]);
        }
        printf("\n");
    }
}

void upgrade_cell(int** grid,int i,int j){
        int neighborhood;
        neighborhood=grid[i-1][j-1]+grid[i-1][j]+grid[i-1][j+1]+grid[i][j-1]+grid[i][j-1]+grid[i][j+1]+grid[i+1][j-1]+grid[i+1][j]+grid[i+1][j+1];
        if (neighborhood==2 || neighborhood==3){
            grid[i][j]=1;
        }else{
            grid[i][j]=0;
        }
    }

void run_episode(int** grid){
    int size=sizeof(grid[0]);
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            upgrade_cell(grid,i,j);
        }
    }

    print_grid(grid);
}

int main(int argc,char** argv ){

    int size;
    int time;
    int in_i;
    int in_j;

    printf("Enter size ");
    scanf("%d",&size);

    printf("Enter number of episodes ");
    scanf("%d",&time);

    int **grid = (int **)malloc(size * sizeof(int *));
    
    for (int i=0;i<size;i++){
        grid[i]=(int*)malloc(size * sizeof(int));
        for (int j=0;j<size;j++){
            grid[i][j]=0;
        }
    }

    char input;
    printf("Do you want to mark an initial value? [y/n]");
    scanf("%c",&input);
    
    while(input=='y'){
    
    printf("Enter i coordinate ");
    scanf("%d",&in_i);

    printf("Enter j coordinate ");
    scanf("%d",&in_j);

    grid[in_i][in_j=1];
    
    printf("Do you want to mark an initial value? [y/n]");
    scanf("%c",&input);
    }

    for (int t=0;t<time;t++){
        run_episode(grid);
    }
   


    return 0;
}