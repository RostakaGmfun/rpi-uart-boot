CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
LDSCRIPT = link.ld
LDFLAGS = -nostdlib
CCFLAGS = -mcpu=arm1176jzf-s -fpic -ffreestanding \
		  -std=gnu99 -Wall -Wno-implicit-function-declaration
OBJECTS = start.o main.o uart.o libc.o loader.o
	
kernel.img: kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel.img

kernel.elf: ${OBJECTS}
	$(LD) -T $(LDSCRIPT) -o kernel.elf $(LDFLAGS) ${OBJECTS}

.c.o:
	$(CC) $(CCFLAGS) $^ -c -o $@

clean:
	rm *.o *.elf

.PHONY: clean

