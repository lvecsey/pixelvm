
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <math.h>
#include <sys/mman.h>
#include <errno.h>

#include "pixelvm.h"

#include "pvm.h"

#include "writefile.h"

int main(int argc, char *argv[]) {

  char *bytecode_fn = argc>1 ? argv[1] : NULL;

  int fd = open(bytecode_fn, O_RDWR);

  struct stat buf;

  void *m;
  
  ssize_t bytes_written;

  int out_fd = 1;

  pvm_t pvm;

  double radius = 0.9;
  double radians;

  long int num_pixels = 200000;

  long int pixelno;

  long int xpos, ypos;

  double x, y;

  double aspect;

  int retval;

  struct pixelvm virt;

  retval = fstat(fd, &buf);
  m = mmap(NULL, buf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (m == MAP_FAILED) {
    perror("mmap");
    return -1;
  }
  
  retval = pixelvm_process(&virt, &pvm, m, buf.st_size);

  munmap(m, buf.st_size);

  close(fd);
  
  bytes_written = writefile(out_fd, pvm.rgb, pvm.xres * pvm.yres * sizeof(color_t));

  return 0;

}

  
