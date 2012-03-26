

volatile unsigned int * const out_addr =  (unsigned int *)0x101f1000;

int console_write(char *s)
{

	while (*s) {
		*out_addr = *s;	
		s++;
	}

}

