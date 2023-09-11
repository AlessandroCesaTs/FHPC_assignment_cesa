#include <stdio.h>
#include<stdlib.h>
#include<time.h>

#include "initialize.h"
#include "run.h"
#include "image_handling.h"

#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec +	\
		  (double)ts.tv_nsec * 1e-9)

int main(int argc,char** argv){
    struct  timespec ts;

    double tstart  = CPU_TIME;
    initialize(50,"test.pgm");
    void* image=NULL;
    int xsize;
    int ysize;
    int maxval;
    char* char_image;
    read_pgm_image(&image,&maxval, &xsize, &ysize, "test.pgm");
    char_image=(char*)image;
    for (int t=0;t<101;t++){
         run_episode_static(&char_image,xsize);
         if (t %100 == 0) {
                 char new_image_name[20];  // Adjust the array size as needed
                snprintf(new_image_name, sizeof(new_image_name), "test_%d.pgm", t);
                write_pgm_image((void*)char_image, xsize, new_image_name);
                }
        }
    double tend = CPU_TIME;
    printf("process took %g \n",tend-tstart);
    return 0;
}
