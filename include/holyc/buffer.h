#ifndef HOLYC_BUFFER
#define HOLYC_BUFFER

typedef struct Buffer {
  char *start;
  char *curr;
  int   size;
} Buffer;

#endif // HOLYC_BUFFER
