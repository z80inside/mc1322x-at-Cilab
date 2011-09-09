#include "mc1322x.h"
#include "config.h"
#include <string.h>
#include <stdio.h>

/* 802.15.4 PSDU is 127 MAX */
/* 2 bytes are the FCS */
/* therefore 125 is the max payload length */

union fcf {
	uint16_t fcf_field;
	struct fcf_ctrl {
		uint16_t ftype:3;
		uint16_t sec_en:1;
		uint16_t fpend:1;
		uint16_t ackreq:1;
		uint16_t panid_comp:1;
		uint16_t rsvd:3;
		uint16_t dest_addr_mode:2;
		uint16_t fver:2;
		uint16_t src_addr_mode:2;
	} fcf_bits;
};

union superframe {
	uint16_t sf_field;
	struct sf_ctrl {
		uint16_t bc_order:4;
		uint16_t sf_order:4;
		uint16_t final_cap:4;
		uint16_t ble:1;
		uint16_t rsvd:1;
		uint16_t pan_coord:1;
		uint16_t asoc:1;
	} sf_bits;
};

struct pkt_info {
	int seq;
	int dst_panid;
	int src_panid;
	long long src;
	long long dst;
	union fcf fcf_info;
	union superframe sf_info;
};

void compose_frame(volatile packet_t *p, struct pkt_info *pi, uint8_t *pay, int len)
{
	int i = 0;
	int j;

	/* Offset = 0 for transmitting */
	p->offset = 0;

	for (j = 0; j < MAX_PAYLOAD_SIZE; j++)
		p->data[j] = 0;

	/** MAC header **/
	/* Frame Control Field */
	p->data[i++] = (uint8_t)(pi->fcf_info.fcf_field & 0x00FF);
	p->data[i++] = (uint8_t)(pi->fcf_info.fcf_field >> 8);
	/* Frame Sequence Number (DSN) */
	p->data[i++] = (uint8_t)(pi->seq);

	switch (pi->fcf_info.fcf_bits.ftype) {
	case 0:
		/* Beacon frame */
		/* PAN ID */
		p->data[i++] = (uint8_t)(pi->dst_panid & 0x000000FF);
		p->data[i++] = (uint8_t)(pi->dst_panid >> 8);
		if (pi->fcf_info.fcf_bits.dest_addr_mode == 2) {
			/* Dest. address (short) */
			p->data[i++] = (uint8_t)(pi->dst & 0x00000000000000FF);
			p->data[i++] = (uint8_t)(pi->dst >> 8);
		}
		if (pi->fcf_info.fcf_bits.src_addr_mode == 2) {
			/* Src. address (short) */
			p->data[i++] = (uint8_t)(pi->src & 0x00000000000000FF);
			p->data[i++] = (uint8_t)(pi->src >> 8);
		}
		/* Superframe specification */
		p->data[i++] = (uint8_t)(pi->sf_info.sf_field & 0x00FF);
		p->data[i++] = (uint8_t)(pi->sf_info.sf_field >> 8);
		/* GTS specification */
		p->data[i++] = 0;
		/* Pending address specification */
		p->data[++i] = 0;
		break;
	case 1:
		/* Data frame */
		/* Dest. PAN ID */
		p->data[i++] = (uint8_t)(pi->dst_panid & 0x000000FF);
		p->data[i++] = (uint8_t)(pi->dst_panid >> 8);
		if (pi->fcf_info.fcf_bits.dest_addr_mode == 2) {
			/* Dest address (short) */
			p->data[i++] = (uint8_t)(pi->dst & 0x00000000000000FF);
			p->data[i++] = (uint8_t)(pi->dst >> 8);
		}
		if (!pi->fcf_info.fcf_bits.panid_comp) {
			/* Src PAN ID */
			p->data[i++] = (uint8_t)(pi->src_panid & 0x000000FF);
			p->data[i++] = (uint8_t)(pi->src_panid >> 8);
		}
		if (pi->fcf_info.fcf_bits.src_addr_mode == 2) {
			/* Src address (short) */
			p->data[i++] = (uint8_t)(pi->src & 0x00000000000000FF);
			p->data[i++] = (uint8_t)(pi->src >> 8);
		}
		break;
	case 2:
		/* ACK */
		break;
	case 3:
		/* MAC command */
		break;
	}
	p->length = len + i;

	/** Payload **/
	for (j = 0; j < len; j++) {
		p->data[i++] = pay[j];
	}
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
	int count = 0;
	struct pkt_info pinfo;

	/* trim the reference osc. to 24MHz */
	trim_xtal();

	uart_init(INC, MOD, SAMP);

	vreg_init();

	maca_init();

	set_channel(0); /* channel 11 */
//	set_power(0x0f); /* 0xf = -1dbm, see 3-22 */
//	set_power(0x11); /* 0x11 = 3dbm, see 3-22 */
	set_power(0x12); /* 0x12 is the highest, not documented */

	*MACA_RXACKDELAY = r;
	
	printf("rx warmup: %d\n\r", (int)(*MACA_WARMUP & 0xfff));

	*MACA_RXEND = end;

	printf("rx end: %d\n\r", (int)(*MACA_RXEND & 0xfff));

	set_prm_mode(AUTOACK);

	while(1) {		
	    		
		/* call check_maca() periodically --- this works around */
		/* a few lockup conditions */
		check_maca();

		while((p = rx_packet())) {
			if(p) {
				printf("RX: ");
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
			case 'd':
				p = get_free_packet();
				if(p) {
					pinfo.seq = count++;
					pinfo.dst_panid = 0xaaaa;
					pinfo.src = 0x2222;
					pinfo.dst = 0x1111;
					pinfo.fcf_info.fcf_bits.src_addr_mode = 2;
					pinfo.fcf_info.fcf_bits.fver = 1;
					pinfo.fcf_info.fcf_bits.dest_addr_mode = 2;
					pinfo.fcf_info.fcf_bits.rsvd = 0;
					pinfo.fcf_info.fcf_bits.panid_comp = 1;
					pinfo.fcf_info.fcf_bits.ackreq = 1;
					pinfo.fcf_info.fcf_bits.fpend = 0;
					pinfo.fcf_info.fcf_bits.sec_en = 0;
					pinfo.fcf_info.fcf_bits.ftype = 1;

					compose_frame(p, &pinfo, "hola", 4);
					
					printf("autoack-tx --- ");
					
					tx_packet(p);				
				}
				break;
			case 'b':
				p = get_free_packet();
				if(p) {
					pinfo.seq = count++;
					pinfo.dst_panid = 0xaaaa;
					pinfo.src = 0x2222;
					pinfo.dst = 0x1111;
					pinfo.fcf_info.fcf_bits.ftype = 0;
					pinfo.fcf_info.fcf_bits.sec_en = 0;
					pinfo.fcf_info.fcf_bits.fpend = 0;
					pinfo.fcf_info.fcf_bits.ackreq = 1;
					pinfo.fcf_info.fcf_bits.panid_comp = 1;
					pinfo.fcf_info.fcf_bits.rsvd = 0;
					pinfo.fcf_info.fcf_bits.dest_addr_mode = 2;
					pinfo.fcf_info.fcf_bits.fver = 1;
					pinfo.fcf_info.fcf_bits.src_addr_mode = 2;
					pinfo.sf_info.sf_bits.bc_order = 0;
					pinfo.sf_info.sf_bits.sf_order = 0;
					pinfo.sf_info.sf_bits.final_cap = 0;
					pinfo.sf_info.sf_bits.ble = 0;
					pinfo.sf_info.sf_bits.rsvd = 0;
					pinfo.sf_info.sf_bits.pan_coord = 1;
					pinfo.sf_info.sf_bits.asoc = 1;

					compose_frame(p, &pinfo, "b", 1);
					
					printf("autoack-tx --- ");
					
					tx_packet(p);				
				}
				break;
			case 'a':
				p = get_free_packet();
				if(p) {
					pinfo.seq = count++;
					pinfo.dst_panid = 0xaaaa;
					pinfo.src = 0x2222;
					pinfo.dst = 0x1111;
					pinfo.fcf_info.fcf_bits.ftype = 2;
					pinfo.fcf_info.fcf_bits.sec_en = 0;
					pinfo.fcf_info.fcf_bits.fpend = 0;
					pinfo.fcf_info.fcf_bits.ackreq = 0;
					pinfo.fcf_info.fcf_bits.panid_comp = 0;
					pinfo.fcf_info.fcf_bits.rsvd = 0;
					pinfo.fcf_info.fcf_bits.dest_addr_mode = 0;
					pinfo.fcf_info.fcf_bits.fver = 0;
					pinfo.fcf_info.fcf_bits.src_addr_mode = 0;

					compose_frame(p, &pinfo, "", 0);
					
					printf("autoack-tx --- ");
					
					tx_packet(p);				
				}
				break;
			}
		}
		
	}
}
