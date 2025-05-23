#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void error_handling(char *);

int main() {
  int fd;
  char buf[] = "Hi,Charles\n";
  fd = open("data.txt", O_CREAT | O_TRUNC | O_WRONLY);
  if (fd == -1)
    error_handling("open() error");
  printf("file descriptor: %d \n", fd);

  if (write(fd, buf, sizeof(buf)) == -1)
    error_handling("write() error");

  close(fd);
  return 0;
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
