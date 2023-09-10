#ifndef IMAGE_HANDLING_H
#define IMAGE_HANDLING_H

void write_pgm_image(void *image,int size,const char *image_name);
void read_pgm_image( void **image, int *size, const char *image_name);

#endif
