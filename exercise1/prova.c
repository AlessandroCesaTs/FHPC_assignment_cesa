#include <stdio.h>
#include <stdlib.h>

void print_grid(int** grid,int size){
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            printf(" %d ",grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}



int evaluate_cell(int** grid,int size,int i,int j){
    int u_i,d_i,l_j,r_j;
    int neighborhood;
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
    neighborhood=grid[u_i][l_j]+grid[u_i][j]+grid[u_i][r_j]+grid[i][l_j]+grid[i][r_j]+grid[d_i][l_j]+grid[d_i][j]+grid[d_i][r_j];
    if (neighborhood==2 || neighborhood==3){
            return 1;
        }else{
            return 0;
        }
}

void upgrade_cell(int** grid,int size,int i,int j){
    grid[i][j]=evaluate_cell(grid,size,i,j);
}


void run_episode_ordered(int** grid,int size){
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            upgrade_cell(grid,size,i,j);
        }
    }

    print_grid(grid,size);
}

void run_episode_static(int** grid,int size){

    int **eval = (int **)malloc(size * sizeof(int *));
    for (int i=0;i<size;i++){
        eval[i]=(int*)malloc(size * sizeof(int));
        for (int j=0;j<size;j++){
            eval[i][j]=evaluate_cell(grid,size,i,j);
        }
    }

    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            grid[i][j]=eval[i][j];
        }
    }
    print_grid(grid,size);

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
     
    print_grid(grid,size);

    char input;
    printf("Do you want to mark an initial value? [y/n] ");
    scanf(" %c",&input);
    
    while(input=='y'){
    
    printf("Enter i coordinate ");
    scanf("%d",&in_i);

    printf("Enter j coordinate ");
    scanf("%d",&in_j);

    grid[in_i][in_j]=1;
    
    printf("Do you want to mark an initial value? [y/n] ");
    scanf(" %c",&input);
    }

    print_grid(grid,size);

    for (int t=0;t<time;t++){
        run_episode_ordered(grid,size);
    }
    return 0;
}