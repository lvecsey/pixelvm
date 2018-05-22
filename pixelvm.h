#ifndef PIXELVM_H
#define PIXELVM_H

#define PVM_SOFFSET 1000
#define PVM_GOFFSET 1001
#define PVM_SCOLOR 2000
#define PVM_SPIXEL 2001

#define PVM_SETRES 3000
#define PVM_ALLOCRGB 3001
#define PVM_FREERGB 3002

#define PVM_FILLRGB 4000 

#define PVM_SLCOUNT 5000
#define PVM_LOOP 5001

#define PVM_SETREGI 5006
#define PVM_SETREGD 5007

#define PVM_MCOS 6000
#define PVM_MSIN 6001
#define PVM_MCOS2 6002
#define PVM_MSIN2 6003

#define PVM_PROJ 7000
#define PVM_PROJ2 7001

#define PVM_SETIZERO 8000
#define PVM_SETDZERO 8001
#define PVM_MULI 8002
#define PVM_MULD 8003
#define PVM_DIVI 8004
#define PVM_DIVD 8005
#define PVM_INCI 8006
#define PVM_INCD 8007
#define PVM_MULI2 8008
#define PVM_MULD2 8009

#include <stdint.h>

struct pixelvm {

  uint64_t a,b,c,d;

  double nda, ndb, ndc, ndd;
  
  uint16_t code;

  uint64_t slcount;
  size_t loop_savedpos;
  
  size_t pos, size;
  
  void *data;
  
};

#include "pvm.h"

int pixelvm_setoffset(pvm_t *pvm, long int xpos, long int ypos);
int pixelvm_setcolor(pvm_t *pvm, long int red, long int green, long int blue);
int pixelvm_setpixel(pvm_t *pvm);

int pixelvm_setresolution(pvm_t *pvm, long int xres, long int yres);
int pixelvm_allocrgb(pvm_t *pvm);
int pixelvm_freergb(pvm_t *pvm);

int pixelvm_fillrgb(pvm_t *pvm);

int pixelvm_slcount(pvm_t *pvm, long int count);
int pixelvm_loop(pvm_t *pvm);

int pixelvm_setregi(pvm_t *pvm, long int value, long int reg_offset, uint64_t *regi_out);
int pixelvm_setregd(pvm_t *pvm, double value, long int reg_offset, double *regd_out);

int pixelvm_mcos(pvm_t *pvm, double radians, long int reg_offset, double *regd_out);
int pixelvm_msin(pvm_t *pvm, double radians, long int reg_offset, double *regd_out);

int pixelvm_setizero(pvm_t *pvm, uint64_t *regi_out);
int pixelvm_setdzero(pvm_t *pvm, double *regd_out);
int pixelvm_muld(pvm_t *pvm, double value, double *regd_out);
int pixelvm_divd(pvm_t *pvm, double value, double *regd_out);

int pixelvm_muld2(pvm_t *pvm, double *regd_in, double *regd_out);
int pixelvm_divd2(pvm_t *pvm, double *regd_in, double *regd_out);

int pixelvm_inci(pvm_t *pvm, uint64_t *regi_out);
int pixelvm_incd(pvm_t *pvm, double *regd_out);

int pixelvm_proj(pvm_t *pvm, double x, double y);

int pixelvm_process(struct pixelvm *virt, pvm_t *pvm, void *data, size_t size);

#endif
