../src/default_lowlevel.o: ../src/default_lowlevel.c \
  ../lib/include/mc1322x.h ../lib/include/isr.h ../lib/include/gpio.h \
  ../lib/include/crm.h ../lib/include/nvm.h ../lib/include/tmr.h \
  ../lib/include/utils.h ../lib/include/kbi.h ../lib/include/maca.h \
  ../lib/include/packet.h ../lib/include/utils.h ../lib/include/packet.h \
  ../lib/include/uart1.h ../lib/include/asm.h
../src/default_lowlevel.c :
  ../lib/include/mc1322x.h ../lib/include/isr.h ../lib/include/gpio.h :
  ../lib/include/crm.h ../lib/include/nvm.h ../lib/include/tmr.h :
  ../lib/include/utils.h ../lib/include/kbi.h ../lib/include/maca.h :
  ../lib/include/packet.h ../lib/include/utils.h ../lib/include/packet.h :
  ../lib/include/uart1.h ../lib/include/asm.h :
