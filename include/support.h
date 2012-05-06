#ifndef __SUPPORT_H
#define __SUPPORT_H
#define log_info_str(str,...) \
			printk("%40s:%20s:%d:%s: "#str"\n", __FILE__, __func__, __LINE__,get_cpsr_info(),##__VA_ARGS__);
//			printk(str, ##__VA_ARGS__);
#define log_info() printk("%s:%s:%d: %s\n", __FILE__, __func__, __LINE__, get_cpsr_info())

char *get_cpsr_info(void);
#endif
