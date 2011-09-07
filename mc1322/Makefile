PROJ := basic_radio_rx
PROJ += basic_radio_tx

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

all: $(addsuffix .bin, $(PROJ))

define build_elf
$(1).elf: src/$(1).c
	arm-none-eabi-gcc $$(CFLAGS) -T $$(LNKSCRIPT) $$(LNKFLAGS) -MMD -o $(1).elf src/$(1).c $$(OBJS) -lm -Ilib/include
endef
$(foreach t, $(PROJ), $(eval $(call build_elf,$(t))))

%.bin: %.elf
	arm-none-eabi-objcopy --gap-fill=0xff -O binary $< $@

clean:
	rm -f *.bin *.elf *.d
