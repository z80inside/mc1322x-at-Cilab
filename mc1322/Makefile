PROGRAM = test
OBJS = test.o lib/GPIO_Interface.o lib/Delay.o lib/buck.o

SRCDIR = src
LIBDIR = lib
INCDIR = include

# Set up cross compiler and toolchain definitions.
CROSS_COMPILE ?= arm-none-eabi-
AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)ranlib

CFLAGS = -g -march=armv4t -mtune=arm7tdmi-s -mlong-calls -msoft-float\
		-mthumb-interwork -fno-strict-aliasing -fno-common\
		-ffixed-r8 -ffunction-sections -ffreestanding -fno-builtin\
		-nodefaultlibs -mthumb -mcallee-super-interworking -Wall -Os\
		-I$(INCDIR) -L$(LIBDIR)

LINKERSCRIPT = mc1322x.lds
LDFLAGS = -T $(LINKERSCRIPT) -nostartfiles -static -export-dynamic -u_start
		

ASFLAGS = -Wa,-gstabs $(CFLAGS)
OBJCOPYFLAGS = --gap-fill=0xff

$(PROGRAM).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROGRAM).elf $(OBJS) -Wl,--start-group \
	lib/libmc1322x.a lib/src-romvars.a -lm -Wl,--end-group
	$(OBJCOPY) $(OBJCOPYFLAGS) -O binary $(PROGRAM).elf $(PROGRAM).bin

%o: $(SRCDIR)/%s
	$(CC) $(ASFLAGS) -MMD -c -o $@ $<

%o: $(SRCDIR)/%c
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

clean:
	rm test.o test.d test.bin test.elf
