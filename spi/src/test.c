#include <mc1322x.h>
#include <redbee-econotag.h>
#include <config.h>
#include <led.h>
#include <rtc.h>
#include <pwm.h>
#include <SPI_Interface.h>
#include <gpio.h>
#include <string.h>

int main(void)
{
	spiErr_t error;
	spiConfig_t config;
	uint32_t data[16];
	uint32_t i;

	rtc_init_osc(0);
	rtc_calibrate();

	*GPIO_FUNC_SEL0 = 0x55155555;
	*GPIO_PAD_DIR0 |= 1 << 11;
	*GPIO_DATA_SET1 |= 1 << 11;
	memset(data, 0, 16 * sizeof(uint32_t));
	data[0] = 0x12345678;
	data[2] = 0x80000000;
	rtc_delay_ms(100);
	error = SPI_Open();
//	while (1) {
		*GPIO_DATA_RESET1 |= 1 << 11;
		rtc_delay_ms(2);
		config.ClkCtrl.Word = 0;
		config.Setup.Word = 0;
		config.ClkCtrl.Bits.DataCount = 32;
		config.ClkCtrl.Bits.ClockCount = 32;
		config.Setup.Bits.ClockFreq = 7;
		config.Setup.Bits.SsDelay = 0;
		config.Setup.Bits.MisoPhase = 0;
		config.Setup.Bits.ClockPhase = 1;
		config.Setup.Bits.ClockPol = 0;
		error = SPI_SetConfig(&config);
		error = SPI_WriteSync(data[0]);
	//	for (i = 0; i < 16; ++i)
	//		error = SPI_WriteSync(data[i]);
		*GPIO_DATA_SET1 |= 1 << 11;
		rtc_delay_ms(1000);
//	}
	error = SPI_Close();
	while (1) ;
}
