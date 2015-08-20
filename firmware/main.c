#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "uart.h"
#include "loader.h"

extern uint8_t __start;
extern uint8_t __end;

extern void memcpy(void *dest, const void *src, size_t num);
extern void printf(const char *fmt, ...);

typedef void (*kentry_t)(void);

#define RAM_END (uint8_t*)0xF2000 /* Should be sufficient space */

void main(uint32_t r0, uint32_t r1, uint32_t atags) {
	(void) r0;
	(void) r1;
	(void) atags;

    uart_init();
	uart_puts("rpi-uart-boot initialised successfully!\r\n");
    uart_puts("waiting for image on UART0\r\n");
    
   loader_main();

	while ( true )
		uart_putc(uart_getc());
}
    
__attribute__((section(".text.relocate"))) void relocate() {
	    uart_init();
        uart_puts("rpi-uart-boot: copiyng itself to higher address\r\n");
        printf("RAM_END %X\r\n", RAM_END);
        
        size_t kernel_size = &__end - &__start;
        uint8_t *new_addr = RAM_END - kernel_size;
        printf("new_addr %X\r\nkernel_size %d=%X\r\n", new_addr, kernel_size, kernel_size);
        printf("=%X %X, =%X %X\r\n",&__start, *(&__start), new_addr, *new_addr);
        memcpy(new_addr, &__start, kernel_size);
        printf("=%X %X, =%X %X\r\n",&__start, *(&__start), new_addr, *new_addr);
        printf("end %x", &__end);
        printf("going to execute relocated code\r\n");
        kentry_t kentry = (kentry_t)new_addr;
        kentry();
}


