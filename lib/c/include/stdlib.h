#ifndef HOLYC_LIB_C_STDLIB
#define HOLYC_LIB_C_STDLIB

#include <mman.h>
#include <stddef.h>

int   exit(int code);
int   atoi(char *str);
long  strtol(const char *str, char **end, int base);
void *malloc(int64_t n);

#endif // HOLYC_LIB_C_STDLIB
