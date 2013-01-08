#include "sysutil.h"
#include "utility.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

void*
asev_sysutil_malloc(unsigned int size)
{
  void* p_ret;
  /* Paranoia - what if we got an integer overflow/underflow? */
  if (size == 0 || size > INT_MAX)
  {
    bug("zero or big size in asev_sysutil_malloc");
  }  
  p_ret = malloc(size);
  if (p_ret == NULL)
  {
    die("malloc");
  }
  return p_ret;
}

void*
asev_sysutil_realloc(void* p_ptr, unsigned int size)
{
  void* p_ret;
  if (size == 0 || size > INT_MAX)
  {
    bug("zero or big size in asev_sysutil_realloc");
  }
  p_ret = realloc(p_ptr, size);
  if (p_ret == NULL)
  {
    die("realloc");
  }
  return p_ret;
}

void
asev_sysutil_free(void* p_ptr)
{
  if (p_ptr == NULL)
  {
    bug("asev_sysutil_free got a null pointer");
  }
  free(p_ptr);
}

unsigned int
asev_sysutil_strlen(const char* p_text)
{
  size_t ret = strlen(p_text);
  /* A defense in depth measure. */
  if (ret > INT_MAX / 8)
  {
    die("string suspiciously long");
  }
  return (unsigned int) ret;
}

char*
asev_sysutil_strdup(const char* p_str)
{
  return strdup(p_str);
}

void
asev_sysutil_memclr(void* p_dest, unsigned int size)
{
  /* Safety */
  if (size == 0)
  {
    return;
  }
  memset(p_dest, '\0', size);
}

void
asev_sysutil_memcpy(void* p_dest, const void* p_src, const unsigned int size)
{
  /* Safety */
  if (size == 0)
  {
    return;
  }
  /* Defense in depth */
  if (size > INT_MAX)
  {
    die("possible negative value to memcpy?");
  }
  memcpy(p_dest, p_src, size);
}


void
asev_sysutil_strcpy(char* p_dest, const char* p_src, unsigned int maxsize)
{
  if (maxsize == 0)
  {
    return;
  }
  strncpy(p_dest, p_src, maxsize);
  p_dest[maxsize - 1] = '\0';
}

int
asev_sysutil_memcmp(const void* p_src1, const void* p_src2, unsigned int size)
{
  /* Safety */
  if (size == 0)
  {
    return 0;
  }
  return memcmp(p_src1, p_src2, size);
}

int
asev_sysutil_strcmp(const char* p_src1, const char* p_src2)
{
  return strcmp(p_src1, p_src2);
}


int
asev_sysutil_atoi(const char* p_str)
{
  return atoi(p_str);
}

filesize_t
asev_sysutil_a_to_filesize_t(const char* p_str)
{
  /* atoll() is C99 standard - but even modern FreeBSD, OpenBSD don't have
   * it, so we'll supply our own
   */
  filesize_t result = 0;
  filesize_t mult = 1;
  unsigned int len = asev_sysutil_strlen(p_str);
  unsigned int i;
  /* Bail if the number is excessively big (petabytes!) */
  if (len > 15)
  {
    return 0;
  }
  for (i=0; i<len; ++i)
  {
    char the_char = p_str[len-(i+1)];
    filesize_t val;
    if (the_char < '0' || the_char > '9')
    {
      return 0;
    }
    val = the_char - '0';
    val *= mult;
    result += val;
    mult *= 10;
  }
  return result;
}

const char*
asev_sysutil_ulong_to_str(unsigned long the_ulong)
{
  static char ulong_buf[32];
  (void) snprintf(ulong_buf, sizeof(ulong_buf), "%lu", the_ulong);
  return ulong_buf;
}

const char*
asev_sysutil_filesize_t_to_str(filesize_t the_filesize)
{
  static char filesize_buf[32];
  if (sizeof(long) == 8)
  {
    /* Avoid using non-standard %ll if we can */
    (void) snprintf(filesize_buf, sizeof(filesize_buf), "%ld",
                    (long) the_filesize);
  }
  else
  {
    (void) snprintf(filesize_buf, sizeof(filesize_buf), "%lld", the_filesize);
  }
  return filesize_buf;
}

const char*
asev_sysutil_double_to_str(double the_double)
{
  static char double_buf[32];
  (void) snprintf(double_buf, sizeof(double_buf), "%.2f", the_double);
  return double_buf;
}


const char*
asev_sysutil_uint_to_octal(unsigned int the_uint)
{
  static char octal_buf[32];
  if (the_uint == 0)
  {
    octal_buf[0] = '0';
    octal_buf[1] = '\0';
  }
  else
  {
    (void) snprintf(octal_buf, sizeof(octal_buf), "0%o", the_uint);
  }
  return octal_buf;
}

unsigned int
asev_sysutil_octal_to_uint(const char* p_str)
{
  /* NOTE - avoiding using sscanf() parser */
  unsigned int result = 0;
  int seen_non_zero_digit = 0;
  while (*p_str != '\0')
  {
    int digit = *p_str;
    if (!isdigit(digit) || digit > '7')
    {
      break;
    }
    if (digit != '0')
    {
      seen_non_zero_digit = 1;
    }
    if (seen_non_zero_digit)
    {
      result <<= 3;
      result += (digit - '0');
    }
    p_str++;
  }
  return result;
}

int
asev_sysutil_toupper(int the_char)
{
  return toupper((unsigned char) the_char);
}

int
asev_sysutil_isspace(int the_char)
{
  return isspace((unsigned char) the_char);
}

int
asev_sysutil_isprint(int the_char)
{
  /* From Solar - we know better than some libc's! Don't let any potential
   * control chars through
   */
  unsigned char uc = (unsigned char) the_char;
  if (uc <= 31)
  {
    return 0;
  }
  if (uc == 177)
  {
    return 0;
  }
  if (uc >= 128 && uc <= 159)
  {
    return 0;
  }
  return isprint(the_char);
}

int
asev_sysutil_isalnum(int the_char)
{
  return isalnum((unsigned char) the_char);
}

int
asev_sysutil_isdigit(int the_char)
{
  return isdigit((unsigned char) the_char);
}

