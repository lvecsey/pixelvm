
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pixelvm.h"

#include "writefile.h"

int main(int argc, char *argv[]) {

  ssize_t bytes_written;

  int out_fd = 1;

  pvm_t pvm;

  pixelvm_setresolution(&pvm, 1600, 900);
  pixelvm_allocrgb(&pvm);

  pixelvm_setcolor(&pvm, 4294967295, 0, 0);
  pixelvm_fillrgb(&pvm);

  bytes_written = writefile(out_fd, pvm.rgb, pvm.xres * pvm.yres * sizeof(color_t));

  return 0;

}

  
