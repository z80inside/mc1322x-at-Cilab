#include "mc1322x.h"

int main(void)
{
	int i;
	rtc_init_osc(0);

	timer_setup_ex(1, 500, 0);
	pwm_init_ex(1, 500, 0, 1);

	while (1) {
		for (i = 0; i < 100; ++i) {
			pwm_duty_ex(1, (int)((65536 * i) / 100));
			rtc_delay_ms(100);
		}
		for (i = 100; i > 0; --i) {
			pwm_duty_ex(1, (int)((65535 * i) / 100));
			rtc_delay_ms(100);
		}
	}
	
	return ;
}
