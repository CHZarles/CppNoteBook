#include "utils.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[]) {
  int serv_sock;
  char message[BUF_SIZE];
  int str_len;

  socklen_t clnt_adr_sz;

  struct sockaddr_in serv_adr, clnt_adr;
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }
  // 创建 UDP 套接字后，向socket的第二个
  // 参数传递 SOCK_DGRAM
  serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
  CHECK_ERROR(serv_sock, "UDP socket creation error");
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  CHECK_ERROR(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)),
              "bind() error");

  while (1) {
    clnt_adr_sz = sizeof(clnt_adr);
    str_len = recvfrom(serv_sock, message, BUF_SIZE, 0,
                       (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
    // 通过上面的函数调用同时获取数据传输端的地址。正是利用该地址进行逆向重传
    sendto(serv_sock, message, str_len, 0, (struct sockaddr *)&clnt_adr,
           clnt_adr_sz);
  }
  close(serv_sock);
  return 0;
}
