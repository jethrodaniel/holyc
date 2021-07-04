int exit(int code) {
  asm("mov  rax, 60");
  asm("syscall");
}

int atoi(char *str) {
  int n = 0,
      i = 0,
      sign = 1;

  if (str[i] == '-') {
    i++;
    sign = -1;
  }

  for (; str[i] >= '0' && str[i] <= '9'; i++)
    n = n * 10 + str[i] - '0';

  return n * sign;
}

// stupid malloc
//
// https://my.eng.utah.edu/~cs4400/malloc.pdf
//
void *malloc(int n) {
  return mmap(
    NULL, // let kernel decide where the mem is
    n,
    PROT_READ | PROT_WRITE | PROT_EXEC,
    MAP_ANONYMOUS | MAP_PRIVATE,
    -1, // map anon
    0   // no offset
  );
}
