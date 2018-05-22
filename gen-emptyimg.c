
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pixelvm.h"

#include "writefile.h"

int main(int argc, char *argv[]) {

  ssize_t bytes_written;

  int out_fd = 1;

  uint16_t code;

  uint64_t a,b,c,d;
  
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
  b = 4294967295;
  c = 4294967295;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));  
  bytes_written = write(out_fd, &a, sizeof(uint64_t));
  bytes_written = write(out_fd, &b, sizeof(uint64_t));
  bytes_written = write(out_fd, &c, sizeof(uint64_t));  
  
  code = PVM_FILLRGB;
  bytes_written = write(out_fd, &code, sizeof(uint16_t));  

  return 0;

}

  
