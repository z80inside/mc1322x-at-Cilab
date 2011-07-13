#include <mc1322x.h>
#include <redbee-econotag.h>
#include <config.h>
#include <led.h>
#include <rtc.h>
#include <pwm.h>
#include <gpio.h>
#include "nvm.h"
#include "put.h"
#include "config.h"

#define DEBUG 1
#if DEBUG
#define dbg_putchr(...) putchr(__VA_ARGS__)
#define dbg_putstr(...) putstr(__VA_ARGS__)
#define dbg_put_hex(...) put_hex(__VA_ARGS__)
#define dbg_put_hex16(...) put_hex16(__VA_ARGS__)
#define dbg_put_hex32(...) put_hex32(__VA_ARGS__)
#else
#define dbg_putchr(...)
#define dbg_putstr(...)
#define dbg_put_hex(...)
#define dbg_put_hex16(...)
#define dbg_put_hex32(...)
#endif

int main(void)
{
	nvmType_t type=0;
	nvmErr_t err;

	rtc_init_osc(0);
	rtc_calibrate();

	uart_init(INC, MOD, SAMP);
	disable_irq(UART1);

	vreg_init();

	dbg_putstr("Detecting internal nvm\n\r");
	err = nvm_detect(gNvmInternalInterface_c, &type);
	rtc_delay_ms(500);

	dbg_putstr("nvm_detect returned: 0x");
	dbg_put_hex(err);
	dbg_putstr(" type is: 0x");
	dbg_put_hex32(type);
	dbg_putstr("\n\r");
	rtc_delay_ms(500);

	dbg_putstr("Detecting external nvm\n\r");
	err = nvm_detect(gNvmExternalInterface_c, &type);

	rtc_delay_ms(500);
	dbg_putstr("nvm_detect returned: 0x");
	dbg_put_hex(err);
	dbg_putstr(" type is: 0x");
	dbg_put_hex32(type);
	dbg_putstr("\n\r");
}
