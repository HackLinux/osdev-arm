#ifndef _FUNCS_H
#define _FUNCS_H
#include "types.h"

#define log_info_str(str,...) \
			printk("%s:%s:%d:%s: "#str"\n", __FILE__, __func__, __LINE__,get_cpsr_info(),##__VA_ARGS__);
//			printk(str, ##__VA_ARGS__);
#define log_info() printk("%s:%s:%d: %s\n", __FILE__, __func__, __LINE__, get_cpsr_info())
void *memcpy(void *dest, const void *src, size_t n);
//void memcpy(void *dst, void *src, int sz);
void *memset(void *s, int c, size_t n);

//void memset(void *, unsigned int sz, char ch);
int memcmp(void *, void *,int sz);
char *strcpy(char *dest, const char *src);

//void strncpy(char *dst, const char *src, int sz);
int strncmp(const char *s1, const char *s2, size_t n);

//char strncmp(const char *dst, const char *src, int sz);
void panic(char *str);
void halt();

#endif
