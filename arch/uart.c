
#include <compiler.h>
#include <interrupt.h>
#include <print.h>

#define UART0_BASE_ADDR 0X101F1000
#define UART0_IMSC      (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x038)))
#define UART0_TICR      (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x044)))

volatile unsigned int * const out_addr =  (unsigned int *)UART0_BASE_ADDR;

#define UART0_IRQ 12

int console_write(char *s)
{
	char *ptr = s;
	while (*s) {
		*out_addr = *s;	
		s++;
	}
	return s - ptr;
}

int uart_handler(int irq, void *data)
{
    unsigned char ch[2]={};
    ch[0] = *out_addr;
    console_write(ch);
    UART0_TICR = 1<<4;


}

void serial_init()
{
    request_irq(UART0_IRQ, IRQ_MODE, uart_handler, 0); 
    UART0_IMSC = 1<<4;
}
 
