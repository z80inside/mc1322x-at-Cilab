#include <mc1322x.h>
#include <redbee-econotag.h>
#include <config.h>
#include <led.h>
#include <rtc.h>
#include <pwm.h>

#define DELAY 400000

int main(void)
{
	rtc_init_osc(0);
	rtc_calibrate();
	pwm_init_ex(0,500, 50000, 1);

	*GPIO_PAD_DIR0 = 0xffffffff;

	uint32_t i = 1;
	uint32_t asc = 1;

	while(1) {
		pwm_duty_ex(0, i);
		(asc) ? i++ : i--;
		if (i == 65535)
			asc = 0;
		if (i == 1)
			asc = 1;
		//*GPIO_DATA0 = 0xffffffff;
		
		//for(i=0; i<DELAY; i++) { continue; }
		//rtc_delay_ms(1000);

		//*GPIO_DATA0 = 0x00000000;

		//for(i=0; i<DELAY; i++) { continue; }
		//rtc_delay_ms(1000);

	}
}
