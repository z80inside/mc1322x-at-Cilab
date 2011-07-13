#include <mc1322x.h>
#include <redbee-econotag.h>
#include <config.h>
#include <led.h>
#include <rtc.h>
#include <pwm.h>
#include <gpio.h>
#include <gpio-util.h>
#include <stdio.h>
#include "nvm.h"
#include "config.h"

int main(void)
{
	int i;
	uint8_t inibuf[4];
	uint8_t c = 0;
	int erase = 0;
	int delay = 2000;

	nvmType_t type_ext=0;
	nvmType_t type_int=0;
	nvmErr_t err;

	rtc_init_osc(0);
	rtc_calibrate();

	//uart_init(INC, MOD, SAMP);
	//disable_irq(UART1);

	vreg_init();

	gpio_select_function(4, 1);
	gpio_select_function(5, 1);
	gpio_select_function(6, 1);
	gpio_select_function(7, 1);
	gpio_select_function(52,1);
	gpio_select_function(59,1);

	gpio_set_pad_dir(52, 1);
	gpio_set_pad_dir(59, 1);

//	printf("Detecting internal nvm\n");
	rtc_delay_ms(500);
	err = nvm_detect(gNvmInternalInterface_c, &type_int);
	rtc_delay_ms(500);
	err = nvm_detect(gNvmExternalInterface_c, &type_ext);
	rtc_delay_ms(500);
	nvm_read(gNvmInternalInterface_c, type_int, inibuf, 0, 4);
	rtc_delay_ms(500);

//	printf("Ini: %c %c %c %c\n", inibuf[0], inibuf[1], inibuf[2],
//		inibuf[3]);
	if (inibuf[0] == 'O' && inibuf[1] == 'K' && inibuf[2] == 'O' 
		&& inibuf[3] == 'K') {
		erase = 1;
		for (i = 0; i < 10000; ++i) {
			nvm_read(gNvmInternalInterface_c, type_int, &c, i, 1);
			nvm_write(gNvmExternalInterface_c, type_ext,
					(uint8_t *)&c, i, 1);
		}
		inibuf[0] = 'N';
		inibuf[1] = 'O';
		inibuf[2] = 'N';
		inibuf[3] = 'O';
		nvm_write(gNvmInternalInterface_c, type_int, inibuf, 0, 4);
		delay = 1000;
	}

	*GPIO_FUNC_SEL0 = 0xffffffff;
	*GPIO_FUNC_SEL1 = 0xffffffff;
	*GPIO_FUNC_SEL2 = 0xffffffff;
	*GPIO_FUNC_SEL3 = 0xffffffff;
	*GPIO_PAD_DIR0 = 0xffffffff;
	*GPIO_PAD_DIR1 = 0xffffffff;
	if (erase) {
		*GPIO_DATA0 = 0xffffffff;
		*GPIO_DATA1 = 0xffffffff;
		rtc_delay_ms(100);
		*GPIO_DATA0 = 0x00000000;
		*GPIO_DATA1 = 0x00000000;
		rtc_delay_ms(100);
		*GPIO_DATA0 = 0xffffffff;
		*GPIO_DATA1 = 0xffffffff;
		rtc_delay_ms(100);
		*GPIO_DATA0 = 0x00000000;
		*GPIO_DATA1 = 0x00000000;
		rtc_delay_ms(100);
	}
	while (1) {
		*GPIO_DATA0 = 0xffffffff;
		*GPIO_DATA1 = 0xffffffff;
		rtc_delay_ms(delay);
		*GPIO_DATA0 = 0x00000000;
		*GPIO_DATA1 = 0x00000000;
		rtc_delay_ms(delay);
	}
	return 0;
}
