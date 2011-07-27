#include "mc1322x.h"
#include "gpio-util.h"
#include "rtc.h"

int main(void)
{
	rtc_init_osc(0);

	gpio_set_pad_dir(44, 1);
	while (1) {
		gpio_set(44);
		rtc_delay_ms(500);
		gpio_reset(44);
		rtc_delay_ms(500);
	}
	
	return ;
}
