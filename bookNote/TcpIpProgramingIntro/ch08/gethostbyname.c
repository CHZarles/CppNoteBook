#include "utils.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int i;
  struct hostent *host;
  if (argc != 2) {
    printf("Usage: %s <hostname>\n", argv[0]);
    exit(1);
  }

  host = gethostbyname(argv[1]);
  if (!host) {
    herror("gethostbyname() error");
    exit(1);
  }
  printf("Official name : %s\n", host->h_name);

  // display other other names
  for (i = 0; host->h_aliases[i]; ++i) {
    printf("Alias %d : %s\n", i + 1, host->h_aliases[i]);
  }

  // check if ipv4
  printf("Address type : %s\n",
         host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");
  // display ip address
  for (i = 0; host->h_addr_list[i]; i++)
    printf("IP addr %d: %s \n", i + 1,
           inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
}
