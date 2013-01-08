#ifndef ASEV_SYSUTIL_H
#define ASEV_SYSUTIL_H

typedef long long filesize_t;

void* asev_sysutil_malloc(unsigned int size);
void* asev_sysutil_realloc(void* p_ptr, unsigned int size);
void asev_sysutil_free(void* p_ptr);

unsigned int asev_sysutil_strlen(const char* p_text);
char* asev_sysutil_strdup(const char* p_str);
void asev_sysutil_memclr(void* p_dest, unsigned int size);
void asev_sysutil_memcpy(void* p_dest, const void* p_src,
                        const unsigned int size);
void asev_sysutil_strcpy(char* p_dest, const char* p_src, unsigned int maxsize);
int asev_sysutil_memcmp(const void* p_src1, const void* p_src2,
                       unsigned int size);
int asev_sysutil_strcmp(const char* p_src1, const char* p_src2);
int asev_sysutil_atoi(const char* p_str);
filesize_t asev_sysutil_a_to_filesize_t(const char* p_str);
const char* asev_sysutil_ulong_to_str(unsigned long the_ulong);
const char* asev_sysutil_filesize_t_to_str(filesize_t the_filesize);
const char* asev_sysutil_double_to_str(double the_double);
const char* asev_sysutil_uint_to_octal(unsigned int the_uint);
unsigned int asev_sysutil_octal_to_uint(const char* p_str);
int asev_sysutil_toupper(int the_char);
int asev_sysutil_isspace(int the_char);
int asev_sysutil_isprint(int the_char);
int asev_sysutil_isalnum(int the_char);
int asev_sysutil_isdigit(int the_char);

#endif /* ASEV_SYSUTIL_H */
