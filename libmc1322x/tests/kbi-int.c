#include <mc1322x.h>
#include <board.h>

#include "tests.h"
#include "config.h"

volatile uint8_t led = 0;

void toggle_led(void)
{
	if (led == 0) {
		*GPIO_DATA_SET0 |= 1 << 11;
		led = 1;
	}
	else {
		*GPIO_DATA_RESET0 |= 1 << 11;
		led = 0;
	}
}

void kbi4_isr(void)
{
	toggle_led();
	clear_kbi_evnt(4);
}

void main(void)
{
	*GPIO_PAD_DIR0 |= 1 << 11;
	*GPIO_DATA_RESET0 |= 1 << 11;

	enable_irq(CRM);
	enable_irq_kbi(4);
	kbi_edge(4);
	enable_ext_wu(4);

	while(1) ;
}
