#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <execinfo.h>

#include "utility.h"

void
bug(const char* p_text)
{
  fprintf(stderr, "Bugger\n%s", p_text);
  print_trace();
  exit(2);
}

void
die(const char* p_text)
{
  bug(p_text);
  exit(2);
}

/* Obtain a backtrace and print it to stdout. */
void
print_trace(void)
{
  void *array[10];
  size_t size;
  char **strings;
  size_t i;
     
  size = backtrace(array, 10);
  strings = backtrace_symbols(array, size);
     
  printf("Obtained %zd stack frames.\n", size);
     
  for (i = 0; i < size; i++)
    printf("%s\n", strings[i]);
     
  free(strings);
}