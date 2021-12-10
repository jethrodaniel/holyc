#ifndef HOLYC_LIB_STDIO
#define HOLYC_LIB_STDIO

#include <lib/stdlib.c>
#include <lib/string.c>
#include <lib/unistd.c>

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

// TODO: return same result as dprintf()
// int dprintf(int fd, char *fmt, ...) {
void dprintf(int fd, char *fmt, ...) {
  int64_t varg1 = 1, varg2 = 2, varg3 = 3, varg4 = 4;

  __asm__("mov %rdx, 0x58(%rsp)"); // varg1
  __asm__("mov %rcx, 0x50(%rsp)"); // varg2
  __asm__("mov %r8,  0x48(%rsp)"); // varg3
  __asm__("mov %r9,  0x40(%rsp)"); // varg4

  int64_t vargs[] = {varg1, varg2, varg3, varg4};

  char *c = fmt;
  int   vars = 0, current = 0;

  while (*c)
    if (*c++ == '%')
      vars++;

  if (vars > 4)
    die("printf with more than 4 args unsupported");

  if (fd < 1 || fd > 2)
    die("dprintf fd must be 1 or 2");

  c = fmt;

  while (*c) {
    if (*c != '%') {
      if (fd == 1)
        putc(*c++);
      if (fd == 2)
        warnc(*c++);
      continue;
    }

    switch (*++c) { // eat '%'
    case 'c':
    case 'd':
    case 'i':
    case 'f':
    case 's':
      if (*c == 's') {
        if (fd == 1)
          print((char *)vargs[current++]);
        if (fd == 2)
          warn((char *)vargs[current++]);
      }

      if (*c == 'd' || *c == 'i') {
        if (fd == 1)
          _printf_print_itoa((int)vargs[current++]);
        if (fd == 2)
          _warnf_print_itoa((int)vargs[current++]);
      }
      if (*c == 'c') {
        if (fd == 1)
          putc((char)vargs[current++]);
        if (fd == 2)
          warnc((char)vargs[current++]);
      }
      if (*c == 'f')
        die("unsupported printf format '%f'");

      break;
    default:
      warn("[abort] unknown printf format '%");
      warnc(*c);
      warn("'\n");
      exit(2);
    }
    c++;
  }
}

void printf(char *fmt, ...) {
  __asm__("mov %r8, %r9");   // varg4
  __asm__("mov %rcx, %r8");  // varg3
  __asm__("mov %rdx, %rcx"); // varg2
  __asm__("mov %rsi, %rdx"); // varg1
  __asm__("mov %rdi, %rsi"); // arg2, fmt
  __asm__("mov $1, %rdi");   // arg1, stdout
  __asm__("call _dprintf");  // dprintf()
}

void warnf(char *fmt, ...) {
  __asm__("mov %r8, %r9");   // varg4
  __asm__("mov %rcx, %r8");  // varg3
  __asm__("mov %rdx, %rcx"); // varg2
  __asm__("mov %rsi, %rdx"); // varg1
  __asm__("mov %rdi, %rsi"); // arg2, fmt
  __asm__("mov $2, %rdi");   // arg1, stderr
  __asm__("call _dprintf");  // dprintf()
}

#endif // HOLYC_LIB_STDIO
