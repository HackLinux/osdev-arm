#include "funcs.h"

void * memset(void *a, int ch, size_t sz)
{
	unsigned int i = 0;
	char *addr = (char *)a;
	while ( i < sz ) {
			addr[i++] = ch;
	}
			

}

char *  strcpy(char *dst, const char *src)
{
	while(*src)
		*dst++ = *src++;
}

void * memcpy(void *dst, const void *src, size_t sz)
{
	char *tdst = (char *)dst;
	char *tsrc = (char *)src;

	while (sz--) {	*tdst++ = *tsrc++; }	

}


int memcmp(void *s1,  void *s2, int sz)
{
	char *t1 = (char *)s1;
	char *t2 = (char *)s2;
	while (sz-- && *t1++ == *t2++);
	return (*--t1 - *--t2);	

}
char *strcat(const char *s1, const char *s2)
{
	while(*s1  && *s1) 
		*s1++ == *s2++;
	return s1;
}



int strncmp(const char *s1, const char *s2, size_t sz)
{
	while(*s1 && sz-- && *s1++ == *s2++);
	return (*--s1 - *--s2); 
}

void panic(char *s)
{
	printk("%s", s);
	while(1);
}

