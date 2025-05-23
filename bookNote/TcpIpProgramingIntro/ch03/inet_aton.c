#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *addr1 = "1.2.3.4";
  char *addr2 = "1.2.3.256";
  int ret = 0;
  struct sockaddr_in addr_inet;
  ret = inet_aton(addr1, &addr_inet.sin_addr);
  if (ret == 0) {
    printf("Error occured! \n");
  } else {
    printf("Network ordered integer addr: %#lx \n", addr_inet.sin_addr.s_addr);
  }

  ret = inet_aton(addr2, &addr_inet.sin_addr);
  if (ret == 0)
    printf("Error occured! \n");
  else
    printf("Network ordered integer addr: %#lx \n", addr_inet.sin_addr.s_addr);
  return 0;
}
