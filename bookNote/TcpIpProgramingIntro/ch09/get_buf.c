#include "utils.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int sock;
  int snd_buf, rev_buf, state;
  socklen_t len;
  sock = socket(PF_INET, SOCK_STREAM, 0);
  len = sizeof(snd_buf);
  state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, &len);
  CHECK_ERROR(state, "getsockopt snd_buf error()");

  len = sizeof(rev_buf);
  state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rev_buf, &len);
  CHECK_ERROR(state, "getsockopt rev_buf error()");
  printf("snd_buf: %d, rev_buf: %d\n", snd_buf, rev_buf);
  state = close(sock);
}
