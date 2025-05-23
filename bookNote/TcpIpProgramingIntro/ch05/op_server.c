#include "utils.hpp"
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define NUMBUFFER 100
#define NUMBER_WISE 10
char number[NUMBER_WISE]; // 只支持包括正负号在内的10位长度的数
uint8_t number_wise = -1; // 记录位数
int numbers[NUMBUFFER];
uint8_t number_count = -1;
char op;

// make sure all charactor vaild
int ch2num(char number[], uint8_t number_wise) {
  assert(number_wise >= 0);
  int res = 0;
  int st_idx = 0;
  if (number[0] == '+' || number[0] == '-') {
    st_idx = 1;
  }
  for (int i = st_idx; i <= number_wise; ++i) {
    assert(number[i] >= '0' && numbers[i] <= '9');
    res = res * 10 + number[i] - '0';
  }

  if (number[0] == '-') {
    res *= -1;
  }
  printf("extract num: %d\n", res);
  return res;
}

int main(int argc, char *argv[]) {
  int serv_sock;
  struct sockaddr_in serv_adr;

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
  // receive format: "10,-5,3343343,-2342343,+12334|*"
  while (1) {
    socklen_t clnt_adr_sz;
    struct sockaddr_in clnt_adr;
    int clnt_sock =
        accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
    // recv number
    int str_len = 0;
    char record[1024];

    int number_end = 0;

    // 每次读一个
    while (read(clnt_sock, record + str_len, 1) != 0) {
      char ch = record[str_len++];

      if (number_end == 1) {
        op = ch;
        // expect end
        record[str_len] = '\0';
        printf("received the whole expression: %s\n", record);
        break;
      } else {

        if (ch == ',' || ch == '|') { // save numbers
          CHECK_ERROR(number_wise, "not vaild sequence");
          numbers[++number_count] = ch2num(number, number_wise);
          // reset number_wise
          number_wise = -1;
          // check if number end
          if (ch == '|')
            number_end = 1;
        } else if (ch >= '0' && ch <= '9' || ch == '+' || ch == '-') {
          number[++number_wise] = ch;
        } else {
          CHECK_ERROR(-1, "Invaild Char");
        }
      }
    }

    // 计算
    int ans = numbers[0];
    for (int i = 1; i <= number_count; ++i) {
      int num = numbers[i];
      if (op == '-') {
        ans -= num;
      } else if (op == '+') {
        ans += num;
      } else if (op == '*') {
        ans *= num;
      }
    }
    printf("Ans: %d\n", ans);
    // send back
    write(clnt_sock, &ans, sizeof(ans));
    close(clnt_sock);
    break;
  }

  close(serv_sock);
  return 0;
}
