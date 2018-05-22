#ifndef PVM_H
#define PVM_H

#include <stdint.h>

typedef struct {

  uint32_t red, green, blue;
  
} color_t;

typedef struct {

  uint64_t state;
  
  long int xpos, ypos;

  long int xres, yres;

  color_t *rgb;
  
  color_t color;
  
  int init : 1;
  
} pvm_t;


#endif
