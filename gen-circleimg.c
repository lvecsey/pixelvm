
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <math.h>

#include "pixelvm.h"

#include "writefile.h"

int main(int argc, char *argv[]) {

  ssize_t bytes_written;

  int out_fd = 1;

  double radius = 0.9;
  double radians;

  long int num_pixels = 200000;

  long int pixelno;

  long int xpos, ypos;

  uint64_t a,b,c,d;

  double nda, ndb, ndc, ndd;
  
  uint16_t code;
  
  double x, y;

  long int xres = 1600, yres = 900;

  double aspect = ((double) xres) / yres;

  long int reg_offset;

  long int reg_offset2;
  
  double value;
  
  code = PVM_SETRES;
  a = 1600;
  b = 900;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));
  bytes_written = write(out_fd, &a, sizeof(uint64_t));
  bytes_written = write(out_fd, &b, sizeof(uint64_t));
  
  code = PVM_ALLOCRGB;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));

  code = PVM_SCOLOR;
  a = 4294967295;
  b = 0;
  c = 0;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));  
  bytes_written = write(out_fd, &a, sizeof(uint64_t));
  bytes_written = write(out_fd, &b, sizeof(uint64_t));
  bytes_written = write(out_fd, &c, sizeof(uint64_t));  
  
  code = PVM_FILLRGB;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));  

  code = PVM_SCOLOR;
  a = 4294967295;
  b = 4294967295;
  c = 4294967295;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));  
  bytes_written = write(out_fd, &a, sizeof(uint64_t));
  bytes_written = write(out_fd, &b, sizeof(uint64_t));
  bytes_written = write(out_fd, &c, sizeof(uint64_t));  

  code = PVM_SETDZERO;
  reg_offset = 3;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));
  bytes_written = write(out_fd, &reg_offset, sizeof(uint64_t));  
  
  code = PVM_SLCOUNT;
  a = num_pixels;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));  
  bytes_written = write(out_fd, &a, sizeof(uint64_t));
  
  {

    code = PVM_SETREGD;
    reg_offset = 0;
    value = 2.0 * M_PI;
    bytes_written = write(out_fd, &code, sizeof(uint16_t));
    bytes_written = write(out_fd, &value, sizeof(double));    
    bytes_written = write(out_fd, &reg_offset, sizeof(uint64_t));
    
    code = PVM_MULD2;
    reg_offset = 3;
    reg_offset2 = 0;
    bytes_written = write(out_fd, &code, sizeof(uint16_t));
    bytes_written = write(out_fd, &reg_offset, sizeof(uint64_t));
    bytes_written = write(out_fd, &reg_offset2, sizeof(uint64_t));    

    code = PVM_DIVD;
    reg_offset = 0;
    value = num_pixels;
    bytes_written = write(out_fd, &code, sizeof(uint16_t));
    bytes_written = write(out_fd, &value, sizeof(double));    
    bytes_written = write(out_fd, &reg_offset, sizeof(uint64_t));
    
    code = PVM_MCOS2;
    reg_offset = 3;
    reg_offset2 = 0;
    bytes_written = write(out_fd, &code, sizeof(uint16_t));  
    bytes_written = write(out_fd, &reg_offset, sizeof(uint64_t));
    bytes_written = write(out_fd, &reg_offset2, sizeof(uint64_t));    

    code = PVM_MSIN2;
    reg_offset = 3;
    reg_offset2 = 1;    
    bytes_written = write(out_fd, &code, sizeof(uint16_t));  
    bytes_written = write(out_fd, &reg_offset, sizeof(uint64_t));
    bytes_written = write(out_fd, &reg_offset2, sizeof(uint64_t));    

    code = PVM_MULD;
    reg_offset = 0;
    value = radius;
    bytes_written = write(out_fd, &code, sizeof(uint16_t));
    bytes_written = write(out_fd, &value, sizeof(double));    
    bytes_written = write(out_fd, &reg_offset, sizeof(uint64_t));

    code = PVM_MULD;
    reg_offset = 1;
    value = radius;
    bytes_written = write(out_fd, &code, sizeof(uint16_t));
    bytes_written = write(out_fd, &value, sizeof(double));    
    bytes_written = write(out_fd, &reg_offset, sizeof(uint64_t));
    
    code = PVM_PROJ;
    bytes_written = write(out_fd, &code, sizeof(uint16_t));  
    
    code = PVM_SPIXEL;
    bytes_written = write(out_fd, &code, sizeof(uint16_t));  

    code = PVM_INCD;
    reg_offset = 3;
    bytes_written = write(out_fd, &code, sizeof(uint16_t));
    bytes_written = write(out_fd, &reg_offset, sizeof(uint64_t));
    
  }
  
  code = PVM_LOOP;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));  
      
  return 0;

}

  
