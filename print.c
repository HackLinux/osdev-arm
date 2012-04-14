//#include "print.h"
#include "stdarg.h"
#include "funcs.h"
#if 0
#define args_list char *

#define _arg_stack_size(type)	\
(((sizeof(type)-1)/sizeof(int)+1)*sizeof(int))

// redefine for 64 bit 
#define args_start(ap, fmt) do {	\
ap = (char *)((char *)&fmt + _arg_stack_size(&fmt));	\
} while (0)

#define args_end(ap)

#define args_next(ap, type) (((type *)(ap+=_arg_stack_size(type)))[-1])
#endif

#define args_list va_list
#define args_start va_start
#define args_end va_end
#define args_next va_arg

static char buf[1024] = {-1};
static int ptr = -1;

void console_write(char *);
/* valid base: 2, 8, 10 */
static int parse_num(unsigned int value, unsigned int base, int width) 
{
	if (!width) width=10;
	unsigned int n = value / base;
	int r = value % base;
	if (r < 0) {
		r += base;
		--n;
	}
	if ((value >= base) && (--width >= 0))
		width = parse_num(n, base, width);
	buf[ptr++] = "0123456789"[r];
	return width;
}

static int parse_hex(unsigned int value, int width) 
{
	if (!width) width= 8;
	int i = width-2;
	int set = 0;
	buf[ptr++] = '0';
	buf[ptr++] = 'x';
	
#if 0
	while (i-- > 0) {
		if (!set && !((value>>(i * 4)) & 0xf)) 
			continue;
		set = 1;
		buf[ptr++] = "0123456789abcdef"[(value>>(i*4))&0xf];
	}
#endif
	if (!value) {
		buf[ptr++] = '0';
		return --i;
	}
	while (i-- > 0) 
		buf[ptr++] = "0123456789abcdef"[(value>>(i*4))&0xf];
	return i;
}

int get_log_level(const char **pbuf)
{
	char *p;
	int log_level = 0;	

	if(*p) {
		p = *(char **)pbuf;
		if( p[0] == '<' && p[1] >= '0' && p[1] <= '7' && p[2] == '>') {
			log_level = p[1] - '0';
			*pbuf += 3;
		}
	}
	return log_level;
}


#define is_digit(c) ((c) >= '0' && (c) <= '9')

static int skip_atoi(const char *s, int *j)
{
    int i=0;
//	print ("in skip \n");
//	print_num(&s[*j], 10);
    while (is_digit(s[*j])){
//		putchar(s[*j]);
        i = i*10 + s[*j] - '0';
		*j = *j + 1;
	}
    return i;
}

static int con_write_enabled = 1;

void disable_con_write()
{
	con_write_enabled = 0;
}

void enable_con_write()
{
	con_write_enabled = 1;
}


/* %s, %c, %x, %d, %% */
//void
char * printk(const char *fmt, ...) 
{
	int i = 0;
	char *s;
	int log_level;

#if 0
	/* must be the same size as enum KP_LEVEL */
/*	struct KPC_STRUCT {
		COLOUR fg;
		COLOUR bg;
	} KPL[] = {
		{BRIGHT_WHITE, BLACK},
		{YELLOW, RED},
	};
*/
#endif

	//log_level = get_log_level(&fmt);
	args_list args;
	args_start(args, fmt);

	ptr = 0;

	for (; fmt[i]; ++i) {
		if ((fmt[i]!='%') && (fmt[i]!='\\')) {
			buf[ptr++] = fmt[i];
			continue;
		} else if (fmt[i] == '\\') {
			/* \a \b \t \n \v \f \r \\ */
			switch (fmt[++i]) {
			case 'a': buf[ptr++] = '\a'; break;
			case 'b': buf[ptr++] = '\b'; break;
			case 't': buf[ptr++] = '\t'; break;
			case 'n': buf[ptr++] = '\n'; break;
			case 'r': buf[ptr++] = '\r'; break;
			case '\\':buf[ptr++] = '\\'; break;
			}
			continue;
		}
		i++;
		int field_width = 0;
		int fw = 0;
		while (is_digit(fmt[i]))
			fw = 10 * fw + fmt[i++] - '0';
		if (fw > 0)
			field_width = fw;

		/* fmt[i] == '%' */
		switch (fmt[i]) {
		case 's':
			s = (char *)args_next(args, char *);
			if (!field_width) {
				while (*s)
					buf[ptr++] = *s++;
			}
			else {
				while (*s && field_width--){
					buf[ptr++] = *s++;
				}
				while(field_width-- > 0)
					buf[ptr++] = ' ';
			}
			break;
		case 'c':
			/* why is int?? */
			buf[ptr++] = (char)args_next(args, int);
			break;
		case 'x':
			field_width = parse_hex((unsigned long)args_next(args, unsigned long), field_width);
			while(field_width-- > 0)
				buf[ptr++] = ' ';
			break;
		case 'd':
			field_width=parse_num((unsigned int)args_next(args, unsigned int), 10, field_width);
			while(field_width-- > 0)
				buf[ptr++] = ' ';
			break;
		case '%':
			buf[ptr++] = '%';
			break;
		default:
			buf[ptr++] = fmt[i];
			break;
		}
	}
	buf[ptr] = '\0';
	args_end(args);
	if (con_write_enabled)
		console_write(buf);
	return buf;
}
