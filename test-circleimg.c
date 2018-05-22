
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <math.h>

#include "pixelvm.h"

#include "writefile.h"

int main(int argc, char *argv[]) {

  ssize_t bytes_written;

  int out_fd = 1;

  pvm_t pvm;

  double radius = 0.9;
  double radians;

  long int num_pixels = 200000;

  long int pixelno;

  long int xpos, ypos;

  double x, y;

  long int xres = 1600, yres = 900;

  double aspect = ((double) xres) / yres;
  
  pixelvm_setresolution(&pvm, xres, yres);
  pixelvm_allocrgb(&pvm);

  pixelvm_setcolor(&pvm, 4294967295, 0, 0);
  pixelvm_fillrgb(&pvm);

  pixelvm_setcolor(&pvm, 4294967295, 4294967295, 4294967295);

  for (pixelno = 0; pixelno < num_pixels; pixelno++) {

    radians = 2.0 * M_PI * pixelno / num_pixels;

    x = cos(radians); y = sin(radians);

    x *= radius;
    y *= radius;
    
    x /= aspect;
    y *= -1.0;
    
    xpos = x * (xres>>1); xpos += xres>>1;
    ypos = y * (yres>>1); ypos += yres>>1;    
    
    pixelvm_setoffset(&pvm, xpos, ypos);
    pixelvm_setpixel(&pvm);

  }
    
  bytes_written = writefile(out_fd, pvm.rgb, pvm.xres * pvm.yres * sizeof(color_t));

  return 0;

}

  
