#include <stdio.h>

int main(int argc, char **argv, char **envp) {
  putc('A');
  putc('\n');

  putc(65); // A
  putc('\n');

  putc(59); // ;
  putc('\n');

  print("print\n");
  warn("warn\n");

  printf("printf with no varargs\n");
  printf("\td | %d, %d, %d, %d\n", 1, 2, 3, 4);
  printf("\ti | %i, %i, %i, %i\n", 1, 2, 3, 4);
  printf("\ts | %s, %s, %s, %s\n", "1", "2", "3", "4");
  printf("\tc | %c, %c, %c, %c\n", '1', '2', '3', '4');
  printf("\t%c\n", 59);
  printf("\t%c %s %s\n", 59, "one", "two");

  print("\n");
  printf("strcmp('a', 'a'):     %d\n", strcmp("a", "a"));
  printf("strcmp('abc', 'abc'): %d\n", strcmp("abc", "abc"));
  printf("strcmp('a', 'b'):     %d\n", strcmp("a", "b"));
  printf("strcmp('ab', 'bc'):   %d\n", strcmp("ab", "bc"));
  printf("strcmp('b', 'a'):     %d\n", strcmp("b", "a"));
  printf("strcmp('bc', 'ab'):   %d\n", strcmp("bc", "ab"));

  return 42;
}
