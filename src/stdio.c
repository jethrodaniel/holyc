// https://blog.nelhage.com/2010/10/amd64-and-va_arg/
void printf(char *fmt, ...) {
  asm("pushq r9");  // arg5
  asm("pushq r8");  // arg4
  asm("pushq rcx"); // arg3
  asm("pushq rdx"); // arg2
  asm("pushq rsi"); // arg1

  char *c = fmt;
  int popped = 0;

  while (*c) {
    if (*c != '%') {
      putc(*c++);
      continue;
    }

    if (popped >= 5)
      die("printf with more than 5 args unsupported");

    switch (*++c) { // eat '%'
    case 's':
      asm("popq rdi");
      popped++;
      asm("call print");
      break;
    case 'd':
      asm("popq rdi");
      popped++;
      asm("call _printf_print_itoa");
      break;
    case 'f':
      die("unsupported printf format '%f'");
    default:
      warn("unknown printf format '");
      putc(*c);
    }
    c++;
  }

  for (int i = 0; i < popped; i++)
    asm("popq r8;"); // can we trash this reg?
}
static void _printf_print_itoa(int n) {
  char buf[] = "     "; // 5
  itoa(n, buf);
  print(buf);
}

void print(char *str) {
  write(STDOUT_FILENO, str, strlen(str));
}

void puts(char *str) {
  print(str);
  write(STDOUT_FILENO, "\n", 1);
}

void putc(char c) {
  write(STDOUT_FILENO, &c, 1);
}

void warn(char *str) {
  write(STDERR_FILENO, str, strlen(str));
}
