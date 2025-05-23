#include "utils.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size;

  char message[] = "Hi,Charles";

  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  // creaet TCP socket
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  CHECK_ERROR(serv_sock, "socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = PF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  // call bind
  CHECK_ERROR(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)),
              "bind() error");

  // call listen
  CHECK_ERROR(listen(serv_sock, 5), "listen() error");

  // call accept
  clnt_addr_size = sizeof(clnt_addr);
  clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
  CHECK_ERROR(clnt_sock, "accept() error");

  write(clnt_sock, message, sizeof(message));
  close(clnt_sock);
  close(serv_sock);
  return 0;
}
