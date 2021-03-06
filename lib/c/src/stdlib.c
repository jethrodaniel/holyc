#include <stdlib.h>

int exit(int code) {
  return (int)syscall(code, 0, 0, 0, 0, 0, SYSCALL_EXIT);
}

int atoi(char *str) {
  int n = 0, i = 0, sign = 1;

  if (str[i] == '-') {
    i++;
    sign = -1;
  }

  for (; str[i] >= '0' && str[i] <= '9'; i++)
    n = n * 10 + str[i] - '0';

  return n * sign;
}

long strtol(const char *str, char **end, int base) {
  int         n = 0, sign = 1;
  const char *c = str;

  if (*c == '-') {
    c++;
    sign = -1;
  }

  for (; *c >= '0' && *c <= '9'; c++)
    n = n * 10 + *c - '0';

  *end = (char *)c;

  return n * sign;
}

// stupid malloc
//
// https://my.eng.utah.edu/~cs4400/malloc.pdf
//
void *malloc(int64_t n) {
  return mmap(NULL, // let kernel decide where the mem is
              n, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
              -1, // map anon
              0   // no offset
  );
}
