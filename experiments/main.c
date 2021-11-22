#include "../lib/crt0.c"
#include "../lib/stdlib.c"
#include "../lib/stdio.c"

#define PAGE_SIZE 4096

int main(int argc, char **argv, char **envp) {
  putc('A');putc('\n');

  int i = 142;
  // dprintf(1, "printf!\n");
  _printf_print_itoa(i);
  print("\n");
  // dprintf(1, "i = %d!\n", i);
  dprintf(1, "%s, %s, %s, %s\n", "one", "two", "three", "four");
  printf("%s, %i, %d, %s\n", "one", 2, 333333333, "four");
  printf("testing\n");

  print("after dprintf\n");

  printf("argc: %d\n",argc);
  for (int i = 0; i < argc ; i++)
    printf("argv[%i] = %s", i, argv[i]);
  print("\n");

  i = 0;
  for (char **env = envp; *env != 0; env++)
    printf("envp[%i] = %s\n", i++, *env);
  print("\n");

  int *lengths = malloc(PAGE_SIZE);

  for (int i = 1; i < argc; i++) {
    lengths[i] = strlen(argv[i]);
  }

  for (int i = 1; i < argc; i++) {
    write(STDOUT, argv[i], lengths[i]);
    putc(' ');
  }

  putc('\n');

  munmap(lengths, PAGE_SIZE);
  return 0;
}
