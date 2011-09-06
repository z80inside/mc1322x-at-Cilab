#SOURCE = src/basic_radio_tx.c
SOURCE = src/basic_radio_rx.c

#PROJ = basic_radio_tx.bin
PROJ = basic_radio_rx.bin

OBJS = lib/start.o
OBJS += lib/default_lowlevel.o
OBJS += lib/isr.o
OBJS += lib/uart1.o
#OBJS += lib/gpio.o
#OBJS += lib/gpio-util.o
#OBJS += lib/rtc.o
#OBJS += lib/crm.o
OBJS += lib/printf.o
OBJS += lib/maca.o
#OBJS += lib/tmr.o
OBJS += lib/nvm.o

CFLAGS = -march=armv4t -mtune=arm7tdmi-s -mlong-calls -msoft-float\
	-mthumb-interwork -fno-strict-aliasing -fno-common -ffixed-r8\
	-ffunction-sections -ffreestanding -fno-builtin -Os -mthumb\
	-mcallee-super-interworking

LNKSCRIPT = mc1322x.lds
LNKFLAGS = -nostartfiles -static -export-dynamic -u_start

$(PROJ).bin: $(PROJ).elf
	arm-none-eabi-objcopy --gap-fill=0xff -O binary $(PROJ).elf $(PROJ).bin
	
$(PROJ).elf: $(SOURCE)
	arm-none-eabi-gcc $(CFLAGS) -T $(LNKSCRIPT) $(LNKFLAGS) -MMD -o $(PROJ).elf $(SOURCE) $(OBJS) -lm -Ilib/include

clean:
	rm *.bin *.elf *.d *.o

