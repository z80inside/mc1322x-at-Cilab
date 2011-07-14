#include "mc1322x.h"
#include "board-conf.h"
#include "config.h"
#include <stdio.h>
#include "rtc.h"
#include "gpio-util.h"
#include "crm.h"
#include "uart1.h"

uint16_t uip_chksum(uint16_t data[], uint8_t len);

uint16_t ipblock[] = {0x0100, 0xf203, 0xf4f5, 0xf6f7};

uint16_t chksum;

void sleep()
{
	*CRM_WU_CNTL = 0x1; /* enable wakeup from wake up timer */
	*CRM_WU_TIMEOUT = 32768*5; /* Using 32KHz crystal, set timer to 5secs */
	*CRM_SLEEP_CNTL = 0xd1; /* Hibernate: retain state, GPIO, RAM pages 0, 1 */
	while((*CRM_STATUS & 0x1) == 0) { continue; }
	*CRM_STATUS = 1; 
	while((*CRM_STATUS & 0x1) == 0) { continue; }
	*CRM_STATUS = 1; 
}

int main(void)
{
//	buck_regulator_setup(1);
	trim_xtal();
	vreg_init();
	rtc_init_osc(rtc_32KHz);

	uart_init(INC, MOD, SAMP);

	gpio_set_pad_dir(44, 1);
	gpio_set_pad_dir(4, 1);

	gpio_set(4);

	while(1) {
		gpio_set(44);
		printf("Going to sleep\n");
//		sleep();
//		printf("Waking up\n");
		rtc_delay_ms(500);
		gpio_reset(44);
		printf("Going to sleep\n");
//		sleep();
//		printf("Waking up\n");
		rtc_delay_ms(500);
	}
}
