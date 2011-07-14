STARTOBJ = start.o
OBJS = default_lowlevel.o
OBJS += uart1.o
OBJS += isr.o
OBJS += crm.o
OBJS += adc.o
OBJS += gpio.o
OBJS += gpio-util.o
OBJS += maca.o
OBJS += nvm.o
OBJS += spi.o
OBJS += printf.o
OBJS += pwm.o
OBJS += rtc.o
OBJS += tmr.o

CFLAGS = -march=armv4t -mtune=arm7tdmi-s -mlong-calls -msoft-float \
		-mthumb-interwork -fno-strict-aliasing -fno-common -ffixed-r8 \
		-ffunction-sections -ffreestanding -fno-builtin -Os
CFLAGS_THUMB = -mthumb -mcallee-super-interworking

all: start.o $(OBJS)

start.o: src/start.S
	arm-none-eabi-gcc -MMD -DUSE_ROM_VARS -c -o $@ $<

%.o: src/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c -o $@ $< -lm -Iinclude

clean:
	rm *.o *.d
