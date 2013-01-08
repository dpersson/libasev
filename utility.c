#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "utility.h"

void
bug(const char* p_text)
{
  fprintf(stderr, "Bugger\n%s", p_text);
  exit(2);
}

void
die(const char* p_text)
{
  bug(p_text);
  exit(2);
}