#ifndef HOLYC_LIB_C_STRING
#define HOLYC_LIB_C_STRING

#include <stddef.h>
#include <stdint.h>

int strlen(char *str);
void reverse(char *str);
void itoa(int n, char *str);
char lower(char c);
int strcmp(const char *s1, const char *s2);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *dest, int c, size_t len);

#endif // HOLYC_LIB_C_STRING
