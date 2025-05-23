#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error_handling(char *);

int main() {
  char *file = "data.txt";
  int fd;
  char buf[100];
  fd = open(file, O_RDONLY);
  if (fd == -1)
    error_handling("open() error");
  printf("file descriptor: %d \n", fd);
  if (read(fd, buf, sizeof(buf)) == -1)
    error_handling("read() error");

  printf("read content: %s", buf);

  close(fd);
  return 0;
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
