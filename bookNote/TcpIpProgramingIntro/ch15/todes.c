#include <fcntl.h>
#include <stdio.h>

int main() {
  FILE *fp;
  int fd =
      open("data.dat", O_WRONLY | O_CREAT |
                           O_TRUNC); // create file and return file descriptor
  if (fd == -1) {
    fputs("file open error", stdout);
    return -1;
  }

  printf("File descriptor: %d\n", fd);
  fp = fdopen(fd, "w"); // open file with FILE pointer
  fputs("Network programming is fun!\n", fp);
  printf("Second file descriptor: %d\n", fileno(fp));
  fclose(fp);
  return 0;
}
