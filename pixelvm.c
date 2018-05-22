
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <errno.h>
#include <math.h>

#include "pixelvm.h"

int pixelvm_setoffset(pvm_t *pvm, long int xpos, long int ypos) {

  pvm->xpos = xpos;
  pvm->ypos = ypos;
  
  return 0;
  
}

int pixelvm_setcolor(pvm_t *pvm, long int red, long int green, long int blue) {

  pvm->color.red = red;
  pvm->color.green = green;
  pvm->color.blue = blue;

  return 0;
  
}

int pixelvm_setpixel(pvm_t *pvm) {

  pvm->rgb[ pvm->ypos * pvm->xres + pvm->xpos ] = pvm->color;

  return 0;
  
}

int pixelvm_setresolution(pvm_t *pvm, long int xres, long int yres) {

  pvm->xres = xres;
  pvm->yres = yres;

  return 0;

}

int pixelvm_allocrgb(pvm_t *pvm) {

  long int num_pixels = pvm->xres * pvm->yres;
  
  pvm->rgb = calloc(num_pixels, sizeof(color_t));
  if (pvm->rgb == NULL) {
    perror("calloc");
    return -1;
  }
  
  return 0;

}

int pixelvm_freergb(pvm_t *pvm) {

  free(pvm->rgb);
  
  return 0;

}

int pixelvm_fillrgb(pvm_t *pvm) {

  long int num_pixels = pvm->xres * pvm->yres;

  long int pixelno;

  for (pixelno = 0; pixelno < num_pixels; pixelno++) {

    pvm->rgb[pixelno] = pvm->color;

  }

  return 0;

}

int pixelvm_setregi(pvm_t *pvm, long int value, long int reg_offset, uint64_t *regi_out) {

  regi_out[0] = value;
  
  return 0;

}

int pixelvm_setregd(pvm_t *pvm, double value, long int reg_offset, double *regd_out) {

  regd_out[0] = value;

  return 0;

}

int pixelvm_mcos(pvm_t *pvm, double radians, long int reg_offset, double *regd_out) {

  regd_out[0] = cos(radians);

  return 0;

}

int pixelvm_msin(pvm_t *pvm, double radians, long int reg_offset, double  *regd_out) {

  regd_out[0] = sin(radians);
  
  return 0;

}

int pixelvm_proj(pvm_t *pvm, double x, double y) {

  long int xpos, ypos;

  double aspect = ((double) pvm->xres) / pvm->yres;

  x /= aspect;
  y *= -1.0;
    
  xpos = x * (pvm->xres >> 1); xpos += pvm->xres >> 1;
  ypos = y * (pvm->yres >> 1); ypos += pvm->yres >> 1;    

  pvm->xpos = xpos;
  pvm->ypos = ypos;

  return 0;
  
}

int pixelvm_setizero(pvm_t *pvm, uint64_t *regi_out) {

  regi_out[0] = 0;

  return 0.0;
  
}

int pixelvm_setdzero(pvm_t *pvm, double *regd_out) {

  regd_out[0] = 0.0;

  return 0.0;
  
}

int pixelvm_muld(pvm_t *pvm, double value, double *regd_out) {

  regd_out[0] *= value;

  return 0;
  
}

int pixelvm_divd(pvm_t *pvm, double value, double *regd_out) {

  regd_out[0] /= value;

  return 0;
  
}

int pixelvm_muld2(pvm_t *pvm, double *regd_in, double *regd_out) {

  regd_out[0] *= regd_in[0];

}

int pixelvm_divd2(pvm_t *pvm, double *regd_in, double *regd_out) {

  regd_out[0] /= regd_in[0];
  
}


int pixelvm_inci(pvm_t *pvm, uint64_t *regi_out) {

  regi_out[0] ++;

  return 0;
  
}

int pixelvm_incd(pvm_t *pvm, double *regd_out) {

  regd_out[0] ++;

  return 0;
  
}

int dump(struct pixelvm *virt) {

  fprintf(stderr, "a=%ld b=%ld c=%ld d=%ld\n", virt->a, virt->b, virt->c, virt->d);

  fprintf(stderr, "nda=%f ndb=%f ndc=%f ndd=%f\n", virt->nda, virt->ndb, virt->ndc, virt->ndd);  

  return 0;

}

int pixelvm_process(struct pixelvm *virt, pvm_t *pvm, void *data, size_t size) {

  uint16_t *local_data = (uint16_t*) data;

  virt->data = data;
  virt->size = size;

  for (virt->pos = 0; virt->pos < size/2; ) {

    virt->code = local_data[virt->pos];

    switch(virt->code) {

    case PVM_SOFFSET:

      memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));
      memcpy(&virt->b, local_data + virt->pos + 5, sizeof(uint64_t));

      pixelvm_setoffset(pvm, virt->a, virt->b);

      virt->pos += 9;
      
      break;
      
    case PVM_GOFFSET: break;
    case PVM_SCOLOR:

      memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));
      memcpy(&virt->b, local_data + virt->pos + 5, sizeof(uint64_t));
      memcpy(&virt->c, local_data + virt->pos + 9, sizeof(uint64_t));            

      pixelvm_setcolor(pvm, virt->a, virt->b, virt->c);

      virt->pos += 13;
      
      break;
    case PVM_SPIXEL:

      pixelvm_setpixel(pvm);

      virt->pos++;
      
      break;

    case PVM_SETRES:

      memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));
      memcpy(&virt->b, local_data + virt->pos + 5, sizeof(uint64_t));      

      pixelvm_setresolution(pvm, virt->a, virt->b);

      virt->pos += 9;
      
      break;

    case PVM_ALLOCRGB:

      pixelvm_allocrgb(pvm);

      virt->pos++;
      
      break;
      
    case PVM_FREERGB:

      pixelvm_freergb(pvm);

      virt->pos++;
      
      break;

    case PVM_FILLRGB:

      pixelvm_fillrgb(pvm);

      virt->pos++;
      
      break;

    case PVM_SLCOUNT:

      memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));
      
      virt->slcount = virt->a;

      virt->pos += 5;
      
      virt->loop_savedpos = virt->pos;
      
      break;

    case PVM_LOOP:

      virt->slcount--;

      virt->pos = (virt->slcount) ? virt->loop_savedpos : virt->pos + 1;
      
      break;
      
    case PVM_SETREGI:

      {
      
	uint64_t *regi_out;

	long int reg_offset;
	
	memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));      
	memcpy(&virt->b, local_data + virt->pos + 5, sizeof(uint64_t));

	reg_offset = virt->b;
	regi_out = (&virt->a) + reg_offset;
      
	pixelvm_setregi(pvm, virt->a, reg_offset, regi_out);

	virt->pos += 9;

      }
      
      break;

      
    case PVM_SETREGD:

      {
      
	double *regd_out;

	long int reg_offset;
	
	memcpy(&virt->nda, local_data + virt->pos + 1, sizeof(double));      
	memcpy(&virt->b, local_data + virt->pos + 5, sizeof(uint64_t));

	reg_offset = virt->b;
	regd_out = (&virt->nda) + reg_offset;
      
	pixelvm_setregd(pvm, virt->nda, reg_offset, regd_out);

	virt->pos += 9;

      }
      
      break;

    case PVM_MCOS:

      {
	
	double *regd_out;

	double radians;
	
	long int reg_offset;
	
	memcpy(&radians, local_data + virt->pos + 1, sizeof(double));
	memcpy(&virt->a, local_data + virt->pos + 5, sizeof(uint64_t));      

	reg_offset = virt->a;

	regd_out = (&virt->nda) + reg_offset;

	pixelvm_mcos(pvm, radians, reg_offset, regd_out);

	virt->pos += 9;

      }
	
      break;
      
    case PVM_MSIN:

      {

	double *regd_out;

	double radians;
	
	long int reg_offset;
	
	memcpy(&radians, local_data + virt->pos + 1, sizeof(double));
	memcpy(&virt->a, local_data + virt->pos + 5, sizeof(uint64_t));      

	reg_offset = virt->a;

	regd_out = (&virt->nda) + reg_offset;

	pixelvm_msin(pvm, radians, virt->a, regd_out);
	virt->pos += 9;

      }
	
      break;

    case PVM_MCOS2:

      {

	double *regd_in;
	
	double *regd_out;

	long int reg_offset;

	long int reg_offset2;	
	
	memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));
	memcpy(&virt->b, local_data + virt->pos + 5, sizeof(uint64_t));	

	reg_offset = virt->a;
	reg_offset2 = virt->b;	

	regd_in = (&virt->nda) + reg_offset;	
	regd_out = (&virt->nda) + reg_offset2;

	pixelvm_mcos(pvm, regd_in[0], reg_offset, regd_out);

	virt->pos += 9;

      }
	
      break;

    case PVM_MSIN2:

      {

	double *regd_in;
	
	double *regd_out;

	long int reg_offset;

	long int reg_offset2;	
	
	memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));
	memcpy(&virt->b, local_data + virt->pos + 5, sizeof(uint64_t));	

	reg_offset = virt->a;
	reg_offset2 = virt->b;	

	regd_in = (&virt->nda) + reg_offset;	
	regd_out = (&virt->nda) + reg_offset2;

	pixelvm_msin(pvm, regd_in[0], reg_offset, regd_out);

	virt->pos += 9;

      }
	
      break;
      
    case PVM_PROJ2:

      memcpy(&virt->nda, local_data + virt->pos + 1, sizeof(double));
      memcpy(&virt->ndb, local_data + virt->pos + 5, sizeof(double));      

      pixelvm_proj(pvm, virt->nda, virt->ndb);
      
      virt->pos += 9;

      break;

    case PVM_PROJ:

      pixelvm_proj(pvm, virt->nda, virt->ndb);
      
      virt->pos++;

      break;

    case PVM_SETIZERO:

      {

	uint64_t *regi_out;
	
	long int reg_offset;

	memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));      

	reg_offset = virt->a;

	regi_out = (&virt->a) + reg_offset;
	
	pixelvm_setizero(pvm, regi_out);

	virt->pos += 5;
	
      }

      break;

    case PVM_SETDZERO:

      {

	double *regd_out;
	
	long int reg_offset;

	memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));      

	reg_offset = virt->a;

	regd_out = (&virt->nda) + reg_offset;
	
	pixelvm_setdzero(pvm, regd_out);

	virt->pos += 5;
	
      }

      break;
      
    case PVM_MULD:

      {

	double *regd_out;

	double value;
	
	long int reg_offset;

	memcpy(&value, local_data + virt->pos + 1, sizeof(double));
	memcpy(&virt->a, local_data + virt->pos + 5, sizeof(uint64_t));

	reg_offset = virt->a;

	regd_out = (&virt->nda) + reg_offset;
	
	pixelvm_muld(pvm, value, regd_out);

	virt->pos += 9;
	
      }

      break;

    case PVM_DIVD:

      {

	double *regd_out;

	double value;
	
	long int reg_offset;

	memcpy(&value, local_data + virt->pos + 1, sizeof(double));
	memcpy(&virt->a, local_data + virt->pos + 5, sizeof(uint64_t));

	reg_offset = virt->a;

	regd_out = (&virt->nda) + reg_offset;
	
	pixelvm_divd(pvm, value, regd_out);

	virt->pos += 9;
	
      }

      break;      

    case PVM_MULD2:

      {

	double *regd_in;
	
	double *regd_out;

	double value;
	
	long int reg_offset;

	long int reg_offset2;

	memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));
	memcpy(&virt->b, local_data + virt->pos + 5, sizeof(uint64_t));	

	reg_offset = virt->a;
	reg_offset2 = virt->b;	

	regd_in = (&virt->nda) + reg_offset;	
	regd_out = (&virt->nda) + reg_offset2;
	
	pixelvm_muld2(pvm, regd_in, regd_out);

	virt->pos += 9;
	
      }

      break;
      
    case PVM_INCI:

      {

	uint64_t *regi_out;

	long int reg_offset;

	memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));

	reg_offset = virt->a;

	regi_out = (&virt->a) + reg_offset;
	
	pixelvm_inci(pvm, regi_out);

	virt->pos += 5;
	
      }

      break;

    case PVM_INCD:

      {

	double *regd_out;

	long int reg_offset;

	memcpy(&virt->a, local_data + virt->pos + 1, sizeof(uint64_t));

	reg_offset = virt->a;

	regd_out = (&virt->nda) + reg_offset;

	pixelvm_incd(pvm, regd_out);

	virt->pos += 5;
	
      }

      break;
      
    default:

      fprintf(stderr, "VM: Unknown code %u\n", local_data[virt->pos]);

      virt->pos++;
      
    }
    
  }
  
  return 0;
  
}
