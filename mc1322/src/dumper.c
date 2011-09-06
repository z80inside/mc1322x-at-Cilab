#include "mc1322x.h"
#include "config.h"
#include <string.h>

int main(void)
{
	char byte;
	uint32_t i;

	rtc_init_osc(1);

	default_vreg_init();
	uart_init(INC, MOD, SAMP);

	for (i = 0; i < 0x14000; i++) {
		memcpy(&byte, (void *)i, 1);
		uart1_putc(byte);
		//rtc_delay_ms(1);
	}
	while (1) ;
}
