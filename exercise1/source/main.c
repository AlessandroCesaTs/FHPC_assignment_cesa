#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include <getopt.h>


#include "initialize.h"
#include "run.h"
#include "image_handling.h"

#define INIT 1
#define RUN  2

#define K_DFLT 100

#define ORDERED 0
#define STATIC  1

char fname_deflt[] = "game_of_life.pgm";

int   action = 0;
int   k      = K_DFLT;
int   e      = ORDERED;
int   n      = 10000;
int   s      = 1;
char *fname  = NULL;

int main(int argc,char** argv){


  char *optstring = "irk:e:f:n:s:";

  int c;
  while ((c = getopt(argc, argv, optstring)) != -1) {
    switch(c) {
      
    case 'i':
      action = INIT; break;
      
    case 'r':
      action = RUN; break;
      
    case 'k':
      k = atoi(optarg); break;

    case 'e':
      e = atoi(optarg); break;

    case 'f':
      fname = (char*)malloc(25*sizeof(char));
      sprintf(fname, "%s", optarg );
      break;

    case 'n':
      n = atoi(optarg); break;

    case 's':
      s = atoi(optarg); break;

    default :
      printf("argument -%c not known\n", c ); break;
    }
  }

if (action==INIT){
	
	/*initialize_test(fname);
	printf("Test playground initialized");
        */

	if (k<100){
	printf("Size too small, use a size >=100\n ");
	}else{
		initialize(k,fname);	
		printf("Playground of size %d initialized \n",k);
	}

}else if(action==RUN){

	if(e!=STATIC && e!=ORDERED){
		printf("Invalid evolution type. Use -e[0|1], 0 for ordered and 1 for static \n");
	}else{
		run(fname,e,n,s);
	}
}else{
	printf("No action inserted. Use -i for initialize and -r for run \n");
}

if ( fname != NULL )
      free ( fname );

  return 0;

}
