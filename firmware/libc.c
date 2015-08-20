/*
 * Necessary libc functionality implemntation goes here
 */
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
    
extern void uart_puts(const char *str);
extern void uart_putc(char c);
extern void halt(void);

size_t strlen(const char *str) {
    size_t sz = 0;
    while(str[sz])
        sz++;
    return sz;
}

void *memset(void *s, int c, size_t n) {
    while(n--)
        *(uint8_t*)s = (uint8_t)c;
    return s;
}

void memcpy(void *dest, const void *src, size_t num) {
    uint8_t *dest8 = (uint8_t*)dest;
    const uint8_t *src8 = (const uint8_t*)src;
    while(num--) {
        *dest8++=*src8++;
    }
}

void abort(void) {
    uart_puts("System aborted\r\n");
    halt();
}

void putchar(char c) {
    uart_putc(c);
}

static char* bf;
static char buf[12];
static unsigned int num;
static char uc;
static char zs;

static void out(char c) {
    *bf++ = c;
    }

static void outDgt(char dgt) {
	out(dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10));
	zs=1;
    }
	
static void divOut(unsigned int div) {
    unsigned char dgt=0;
	num &= 0xffff; // just for testing the code  with 32 bit ints
	while (num>=div) {
		num -= div;
		dgt++;
		}
	if (zs || dgt>0) 
		outDgt(dgt);
    }	

void printf(char *fmt, ...)
{
	va_list va;
	char ch;
	char* p;
	
	va_start(va,fmt);
	
	while ((ch=*(fmt++))) {
		if (ch!='%') {
			putchar(ch);
        }
		else {
	    	char lz=0;
			char w=0;
			ch=*(fmt++);
			if (ch=='0') {
				ch=*(fmt++);
				lz=1;
			}
			if (ch>='0' && ch<='9') {
				w=0;
				while (ch>='0' && ch<='9') {
					w=(((w<<2)+w)<<1)+ch-'0';
					ch=*fmt++;
					}
			}
			bf=buf;
			p=bf;
			zs=0;
			switch (ch) {
				case 0: 
					goto abort;
				case 'u':
				case 'd' : 
					num=va_arg(va, unsigned int);
					if (ch=='d' && (int)num<0) {
						num = -(int)num;
						out('-');
						}
					divOut(10000);
					divOut(1000);
					divOut(100);
					divOut(10);
					outDgt(num);
					break;
				case 'x': 
				case 'X' : 
				    uc= ch=='X';
					num=va_arg(va, unsigned int);
					divOut(0x1000);
					divOut(0x100);
					divOut(0x10);
					outDgt(num);
					break;
				case 'c' : 
					out((char)(va_arg(va, int)));
					break;
				case 's' : 
					p=va_arg(va, char*);
					break;
				case '%' :
					out('%');
				default:
					break;
			}
			*bf=0;
			bf=p;
			while (*bf++ && w > 0)
				w--;
			while (w-- > 0) 
				putchar(lz ? '0' : ' ');
			while ((ch= *p++))
				putchar(ch);
			}
		}
	abort:;
	va_end(va);
	}


