
#include <stdio.h>
#include <stdlib.h>

#define CHECK_ERROR(ret, message)                                              \
  do {                                                                         \
    if ((ret) == -1) {                                                         \
      error_handling(message);                                                 \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
