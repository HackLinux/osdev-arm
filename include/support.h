#ifndef __SUPPORT_H
#define __SUPPORT_H
#define log_info_str(str,...) \
			printk("%15s:%15s:%d:%s: "str"\n", __FILE__, __func__, __LINE__,get_cpsr_info(),##__VA_ARGS__);
//			printk(str, ##__VA_ARGS__);
#define log_info() printk("%s:%s:%d: %s\n", __FILE__, __func__, __LINE__, get_cpsr_info())

char *get_cpsr_info(void);
#define ABRT 	10111
#define FIQ	10001
#define IRQ	10010
#define SVC	10011
#define SYS	11111
#define UND	11011
#define USR	10000

static char str_mode[][4]      = {"", "abt", "fiq",  "irq", "svc", "sys", "und", "usr"};
static char modes[4][4]         = {
                        {7, 0, 0, 0},
                        {2, 0, 0, 0},
                        {3, 0, 0, 0},
                        {4, 1, 6, 5},
                        };

#define print_mode(m) str_mode[modes[(m)&0x3][((m)&0xc)>>2]]
#define get_mode(m) ((m)&0x1f)
#endif
