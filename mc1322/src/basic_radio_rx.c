#include "mc1322x.h"
#include "config.h"
#include <stdio.h>

/* 802.15.4 PSDU is 127 MAX */
/* 2 bytes are the FCS */
/* therefore 125 is the max payload length */
#define PAYLOAD_LEN 16
#define DELAY 100000

void maca_rx_callback(volatile packet_t *p) {
	(void)p;
}

void main(void) {
	volatile packet_t *p;
	volatile uint8_t t=20;
	uint8_t chan;
	char c;

	/* trim the reference osc. to 24MHz */
	trim_xtal();

	uart_init(INC, MOD, SAMP);

	vreg_init();

	maca_init();

	set_power(0x0f); /* 0dbm */
	chan = 0;
	set_channel(chan); /* channel 11 */

	*MACA_MACPANID = 0xaaaa;
	*MACA_MAC16ADDR = 0x1111;
	*MACA_TXACKDELAY = 68; /* 68 puts the tx ack at about the correct spot */
	set_prm_mode(AUTOACK);

	while(1) {		

		/* call check_maca() periodically --- this works around */
		/* a few lockup conditions */
		check_maca();

		if((p = rx_packet())) {
			/* print and free the packet */
			printf("autoack-rx --- ");
			free_packet(p);
		}

		if(uart1_can_get()) {
			c = uart1_getc();
			if(c == 'z') t++;
			if(c == 'x') t--;
			*MACA_TXACKDELAY = t;
			printf("tx ack delay: %d\n\r", t);
		}

	}
}
