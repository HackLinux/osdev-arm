#include <funcs.h>

void *memset(void *a, int ch, size_t sz)
{
	unsigned int i = 0;
	char *addr = (char *)a;
	while ( i < sz ) {
		addr[i++] = ch;
	}
	return a;
			

}

char *strcpy(char *dst, const char *src)
{
	char *t = dst;
	while(*src)
		*dst++ = *src++;
	return t;
}

char *strncpy(char *dst, const char *src, size_t sz)
{
	char *t = dst;
	while(*src && sz--)
		*dst++ = *src++;
	return t;
}

void *memcpy(void *dst, const void *src, size_t sz)
{
	char *t = dst;
	char *tdst = (char *)dst;
	char *tsrc = (char *)src;

	while (sz--) {	*tdst++ = *tsrc++; }	
	return t;
}


int memcmp(void *s1,  void *s2, int sz)
{
	char *t1 = (char *)s1;
	char *t2 = (char *)s2;
	while (sz-- && *t1++ == *t2++);
	return (*--t1 - *--t2);	

}
char *strcat(char *s1, const char *s2)
{
	char *t = s1;
	while(*s1  && *s1) 
		*s1++ = *s2++;
	return t;
}



int strncmp(const char *s1, const char *s2, size_t sz)
{
	while(*s1 && sz-- && *s1++ == *s2++);
	return (*--s1 - *--s2); 
}

