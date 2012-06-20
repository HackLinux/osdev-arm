#ifndef __FUNCS_H
#define __FUNCS_H
#include "types.h"

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);

//void memset(void *, unsigned int sz, char ch);
int memcmp(void *, void *,int sz);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);

//void strncpy(char *dst, const char *src, int sz);
int strncmp(const char *s1, const char *s2, size_t n);

//char strncmp(const char *dst, const char *src, int sz);
void panic(char *str);
void halt();

#endif
