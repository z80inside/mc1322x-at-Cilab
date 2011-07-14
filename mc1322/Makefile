SOURCE = src/test.c

OBJS = lib/start.o
OBJS += lib/default_lowlevel.o
OBJS += lib/isr.o
OBJS += lib/uart1.o
OBJS += lib/gpio.o
OBJS += lib/gpio-util.o
OBJS += lib/rtc.o

LNKSCRIPT = mc1322x.lds
LNKFLAGS = -nostartfiles -static -export-dynamic -u_start

test.bin: test.elf
	arm-none-eabi-objcopy --gap-fill=0xff -O binary test.elf test.bin
	
test.elf: $(SOURCE)
	arm-none-eabi-gcc -T $(LNKSCRIPT) $(LNKFLAGS) -o test.elf $(SOURCE) $(OBJS) -Ilib/include

clean:
	rm *.bin *.elf *.d *.o

