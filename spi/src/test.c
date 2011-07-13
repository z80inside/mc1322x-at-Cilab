/*
 * Copyright (c) Ricardo Cañuelo Navarro
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <mc1322x.h>
#include <redbee-econotag.h>
#include <config.h>
#include <led.h>
#include <rtc.h>
#include <pwm.h>
#include <SPI_Interface.h>
#include <gpio.h>
#include <gpio-util.h>
#include <string.h>

int main(void)
{
	spiErr_t error;
	spiConfig_t config;
	uint32_t data;

	CRM->SYS_CNTLbits.PWR_SOURCE = 1;
	CRM->VREG_CNTLbits.BUCK_CLKDIV = 0xf;
	CRM->VREG_CNTLbits.BUCK_SYNC_REC_EN = 1;
	CRM->VREG_CNTLbits.BUCK_EN = 1;
	while (!CRM->STATUSbits.VREG_BUCK_RDY)
		continue;
	CRM->VREG_CNTLbits.VREG_1P5V_EN = 3;
	CRM->VREG_CNTLbits.VREG_1P5V_SEL = 3;
	CRM->VREG_CNTLbits.VREG_1P8V_EN = 1;
	while (!CRM->STATUSbits.VREG_1P5V_RDY || !CRM->STATUSbits.VREG_1P8V_RDY)
		continue;

	/* Initialize RTC */
	rtc_init_osc(0);
	rtc_calibrate();

#if 0
	/* Configure SPI pins to Functional 1
	 * NOTE: GPIOs 0 to 15 except 11 (TMR3) are set in Functional 1 here */
	*GPIO_FUNC_SEL0 = 0x55155555;

	/* Set GPIO11 (TMR3) to output and HIGH */
	*GPIO_PAD_DIR0 |= 1 << 11;
	*GPIO_DATA_SET0 |= 1 << 11;

	/* Output data via SPI ten times with one second pauses in between
	 * GPIO11 (TMR3) is used as an additional Slave Select */
	data = 0x12345678;
	error = SPI_Open();
	for (i = 0; i < 10; ++i) {
		*GPIO_DATA_RESET0 |= 1 << 11;
		rtc_delay_ms(2);
		config.ClkCtrl.Word = 0;
		config.Setup.Word = 0;
		config.ClkCtrl.Bits.DataCount = 32;
		config.ClkCtrl.Bits.ClockCount = 32;
		config.Setup.Bits.ClockFreq = 7;
		config.Setup.Bits.ClockPhase = 1;
		error = SPI_SetConfig(&config);
		error = SPI_WriteSync(data);
		*GPIO_DATA_SET0 |= 1 << 11;
		rtc_delay_ms(1000);
	}
	error = SPI_Close();
#endif
	gpio_set_pad_dir(44, 1);
	while (1) {
		gpio_set(44);
		rtc_delay_ms(500);
		gpio_reset(44);
		rtc_delay_ms(500);
	}
}
