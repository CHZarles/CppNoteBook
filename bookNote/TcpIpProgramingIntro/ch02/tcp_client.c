#include "utils.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 100

int main(int argc, char *argv[]) {

  int sock;
  struct sockaddr_in serv_addr;
  char message[30];
  int str_len = 0;

  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
  }

  // 若前两个参数使用PF_INET 和 SOCK_STREAM，则可以省略第三个参数 IPPROTO_TCP
  sock = socket(PF_INET, SOCK_STREAM, 0);
  CHECK_ERROR(sock, "socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  CHECK_ERROR(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)),
              "connect() error!");

  int idx = 0, read_len = 0;
  while (read_len = read(sock, &message[idx++], 1)) {
    CHECK_ERROR(read_len, "read() error!");
    str_len += read_len;
  }
  printf("Message from server : %s \n", message);
  printf("Function read call count: %d \n", str_len);
  close(sock);
  return 0;
}
