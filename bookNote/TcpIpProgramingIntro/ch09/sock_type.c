#include "utils.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int tcp_sock, udp_sock;
  int sock_type;
  socklen_t optlen;
  int state;

  optlen = sizeof(sock_type);
  tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
  CHECK_ERROR(tcp_sock, "tcp socket error()");
  udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
  CHECK_ERROR(udp_sock, "udp socket error()");

  state =
      getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
  CHECK_ERROR(state, "getsockopt tcp error()");
  printf("Socket type of tcp_sock: %d\n", sock_type);

  state =
      getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
  CHECK_ERROR(state, "getsockopt udp error()");
  printf("Socket type of udp_sock: %d\n", sock_type);
  return 0;
}
