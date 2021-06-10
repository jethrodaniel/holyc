// = holyc
//
// == references
//
// - Terry Davis's mini compiler demo:
//   https://github.com/cia-foundation/TempleOS/blob/archive/Demo/Lectures/MiniCompiler.HC

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  char buf[255];
  int num_read;

  while ((num_read = read(STDIN_FILENO, buf, 255)) > 0) {
    write(STDOUT_FILENO, &buf, num_read);
  }

  return EXIT_SUCCESS;
}
