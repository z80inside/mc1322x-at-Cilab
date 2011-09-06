#include "mc1322x.h"
#include "config.h"
#include <stdio.h>

/* 802.15.4 PSDU is 127 MAX */
/* 2 bytes are the FCS */
/* therefore 125 is the max payload length */
#define PAYLOAD_LEN 16
#define DELAY 100000

void fill_packet(volatile packet_t *p) {
	static volatile uint8_t count=0;

	p->length = 16;
	p->offset = 0;
	p->data[0] = 0x71;  /* 0b 10 01 10 000 1 1 0 0 001 data, ack request, short addr */
	p->data[1] = 0x98;  /* 0b 10 01 10 000 1 1 0 0 001 data, ack request, short addr */
	p->data[2] = count++; /* dsn */
	p->data[3] = 0xaa;    /* pan */
	p->data[4] = 0xaa;
	p->data[5] = 0x11;    /* dest. short addr. */
 	p->data[6] = 0x11;
	p->data[7] = 0x22;    /* src. short addr. */
 	p->data[8] = 0x22;

	/* payload */
	p->data[9] = 'a';
	p->data[10] = 'c';
	p->data[11] = 'k';
	p->data[12] = 't';
	p->data[13] = 'e';
	p->data[14] = 's';
	p->data[15] = 't';
}

void maca_tx_callback(volatile packet_t *p) {
	switch(p->status) {
	case 0:
		printf("TX OK\n\r");
		break;
	case 3:
		printf("CRC ERR\n\r");
		break;
	case 5:
		printf("NO ACK\n\r");
		break;
	default:
		printf("unknown status: %d\n", (int)p->status);
	}
}

void main(void) {
	volatile packet_t *p;
	char c;
	uint16_t r=30; /* start reception 100us before ack should arrive */
	uint16_t end=180; /* 750 us receive window*/

	/* trim the reference osc. to 24MHz */
	trim_xtal();

	uart_init(INC, MOD, SAMP);

	vreg_init();

	maca_init();

	set_channel(0); /* channel 11 */
//	set_power(0x0f); /* 0xf = -1dbm, see 3-22 */
//	set_power(0x11); /* 0x11 = 3dbm, see 3-22 */
	set_power(0x12); /* 0x12 is the highest, not documented */

        /* sets up tx_on, should be a board specific item */
	//GPIO->FUNC_SEL_44 = 1;	 
	//GPIO->PAD_DIR_SET_44 = 1;	 

	//GPIO->FUNC_SEL_45 = 2;	 
	//GPIO->PAD_DIR_SET_45 = 1;	 

	*MACA_RXACKDELAY = r;
	
	printf("rx warmup: %d\n\r", (int)(*MACA_WARMUP & 0xfff));

	*MACA_RXEND = end;

	printf("rx end: %d\n\r", (int)(*MACA_RXEND & 0xfff));

	set_prm_mode(AUTOACK);

	//print_welcome("rftest-tx");

	while(1) {		
	    		
		/* call check_maca() periodically --- this works around */
		/* a few lockup conditions */
		check_maca();

		while((p = rx_packet())) {
			if(p) {
				printf("RX: ");
				//print_packet(p);
				free_packet(p);
			}
		}

		if(uart1_can_get()) {
			c = uart1_getc();

			switch(c) {
			case 'z':
				r++;
				if(r > 4095) { r = 0; }
				*MACA_RXACKDELAY = r;
				printf("rx ack delay: %d\n\r", r);
				break;
			case 'x':
				if(r == 0) { r = 4095; } else { r--; }
				*MACA_RXACKDELAY = r;
				printf("rx ack delay: %d\n\r", r);
				break;
			case 'q':
				end++;
				if(r > 4095) { r = 0; }
				*MACA_RXEND = end;
				printf("rx end: %d\n\r", end);
				break;
			case 'w':
				end--;
				if(r == 0) { r = 4095; } else { r--; }
				*MACA_RXEND = end;
				printf("rx end: %d\n\r", end);
				break;
			default:
				p = get_free_packet();
				if(p) {
					fill_packet(p);
					
					printf("autoack-tx --- ");
					//print_packet(p);
					
					tx_packet(p);				
				}
				break;
			}
		}
		
	}
}
