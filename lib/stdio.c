#ifndef HOLYC_LIB_STDIO
#define HOLYC_LIB_STDIO

#include <lib/unistd.c>
#include <lib/string.c>

// putc()

void fputc(int fd, char c) {
  write(fd, &c, 1);
}

void putc(char c) {
  fputc(STDOUT_FILENO, c);
}

void warnc(char c) {
  fputc(STDERR_FILENO, c);
}

// print()

void fprint(int fd, char *str) {
  write(fd, str, strlen(str));
}

void print(char *str) {
  fprint(STDOUT_FILENO, str);
}

void warn(char *str) {
  fprint(STDERR_FILENO, str);
}

// misc

void die(char *str) {
  warn("[error]: ");
  warn(str);
  warn("\n");
}

// printf()

int dprintf(int fd, char *fmt, ...) {
  asm("pushq %r9");  // varg4
  asm("pushq %r8");  // varg3
  asm("pushq %rcx"); // varg2
  asm("pushq %rdx"); // varg1

  char *c = fmt;
  int popped = 0;

  while (*c) {
    if (*c != '%') {
      if (fd == 1)
        putc(*c++);
      if (fd == 2)
        warnc(*c++);
      continue;
    }

    if (popped > 4)
      die("printf with more than 4 args unsupported");

    switch (*++c) { // eat '%'
      case 'c':
      case 'd':
      case 'f':
      case 's':
        asm("popq %rdi");
        popped++;

        if (*c == 's') {
          if (fd == 1)
            asm("call _print");
          if (fd == 2)
            asm("call _warn");
        }

        if (*c == 'd') {
          if (fd == 1)
            asm("call __printf_print_itoa");
          if (fd == 2)
            asm("call __warnf_print_itoa");
        }
        if (*c == 'c') asm("call _putc");
        if (*c == 'f') die("unsupported printf format '%f'");

        break;
      default:
        warn("unknown printf format '%");
        putc(*c); // todo
        warn("'");
    }
    c++;
  }

  for (int i = popped; i < 4; i++)
    asm("popq %r8"); // can we trash this reg?

  // xor r8, r8
  asm("xor %r8, %r8"); // can we trash this reg?
}

void printf(char *fmt, ...) {
  // mov r9, r8
  asm("mov %r8, %r9");   // varg4
  asm("mov %rcx, %r8");  // varg3
  asm("mov %rdx, %rcx"); // varg2
  asm("mov %rsi, %rdx"); // varg1
  asm("mov %rdi, %rsi"); // arg2, fmt
  asm("mov $1, %rdi");   // arg1, stdout
  asm("call _dprintf"); // dprintf()
}

void warnf(char *fmt, ...) {
  asm("mov %r8, %r9");   // varg4
  asm("mov %rcx, %r8");  // varg3
  asm("mov %rdx, %rcx"); // varg2
  asm("mov %rsi, %rdx"); // varg1
  asm("mov %rdi, %rsi"); // arg2, fmt
  asm("mov $2, %rdi");   // arg1, stderr
  asm("call _dprintf"); // dprintf()
}

// todo: get len of number, then malloc
//
void _printf_print_itoa(int n) {
  char buf[] = "                "; // 5
  itoa(n, buf);
  print(buf);
}

void _warnf_print_itoa(int n) {
  char buf[] = "                "; // 5
  itoa(n, buf);
  warn(buf);
}

#endif // HOLYC_LIB_STDIO
