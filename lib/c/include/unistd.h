#ifndef HOLYC_LIB_C_UNISTD
#define HOLYC_LIB_C_UNISTD

#define STDIN_FILENO  0
#define STDIN         STDIN_FILENO
#define STDOUT_FILENO 1
#define STDOUT        STDOUT_FILENO
#define STDERR_FILENO 2
#define EXIT_SUCCESS  0

#include <stdint.h>
#include <syscall.h>

int64_t write(int fd, void *buf, size_t length);
int64_t read(int fd, char *buf, size_t length);

typedef int64_t pid_t;

pid_t getpid();
pid_t fork();

#endif // HOLYC_LIB_C_UNISTD
