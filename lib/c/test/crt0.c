#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp) {
  printf("[TEST] %s\n", __FILE__);

  printf("\targc: %d\n", argc);
  for (int i = 0; i < argc; i++)
    printf("\targv[%i] = %s", i, argv[i]);
  printf("\n\n");

  int i = 0;
  for (char **env = envp; *env != 0; env++)
    printf("\tenvp[%i] = %s\n", i++, *env);
  printf("\n");

  return 0;
}
