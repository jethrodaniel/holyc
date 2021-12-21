#ifndef HOLYC_LIB_C_STDIO
#define HOLYC_LIB_C_STDIO

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void fputc(int fd, char c);
void putc(char c);
void warnc(char c);
void fprint(int fd, char *str);
void print(char *str);
void warn(char *str);

void die(char *str);
void _printf_print_itoa(int n);

void _warnf_print_itoa(int n);

void dprintf(int fd, char *fmt, ...);

void printf(char *fmt, ...);
void warnf(char *fmt, ...);

#endif // HOLYC_LIB_C_STDIO
