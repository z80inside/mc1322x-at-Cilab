SOURCE = src/fade.c

OBJS = lib/start.o
OBJS += lib/default_lowlevel.o
OBJS += lib/isr.o
OBJS += lib/uart1.o
OBJS += lib/gpio.o
OBJS += lib/gpio-util.o
OBJS += lib/rtc.o
OBJS += lib/crm.o
OBJS += lib/printf.o
OBJS += lib/pwm.o
OBJS += lib/tmr.o

CFLAGS = -march=armv4t -mtune=arm7tdmi-s -mlong-calls -msoft-float\
	-mthumb-interwork -fno-strict-aliasing -fno-common -ffixed-r8\
	-ffunction-sections -ffreestanding -fno-builtin -Os -mthumb\
	-mcallee-super-interworking

LNKSCRIPT = mc1322x.lds
LNKFLAGS = -nostartfiles -static -export-dynamic -u_start

test.bin: test.elf
	arm-none-eabi-objcopy --gap-fill=0xff -O binary test.elf test.bin
	
test.elf: $(SOURCE)
	arm-none-eabi-gcc $(CFLAGS) -T $(LNKSCRIPT) $(LNKFLAGS) -MMD -o test.elf $(SOURCE) $(OBJS) -lm -Ilib/include

clean:
	rm *.bin *.elf *.d *.o

