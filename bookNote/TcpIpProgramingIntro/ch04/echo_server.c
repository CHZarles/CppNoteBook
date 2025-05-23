
#include "utils.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  int serv_sock, clnt_sock;
  char message[BUF_SIZE];
  int str_len, i;

  struct sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;

  if (argc != 2) {
    printf("Usage : %s <port> \n", argv[0]);
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  CHECK_ERROR(serv_sock, "socket() error");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  CHECK_ERROR(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)),
              "bind() error");

  CHECK_ERROR(listen(serv_sock, 5), "listen() error");

  clnt_adr_sz = sizeof(clnt_adr);
  // call 5 times
  for (i = 0; i < 5; ++i) {
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
    CHECK_ERROR(clnt_sock, "accept() error");

    printf("Connect client %d \n", i + 1);

    while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
      write(clnt_sock, message, str_len);
    }
    close(clnt_sock);
  }

  close(serv_sock);
  return 0;
}
