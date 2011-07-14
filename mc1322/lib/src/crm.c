
#include "crm.h"

void set_power_source(int src)
{
	CRM->SYS_CNTLbits.PWR_SOURCE = src;
}

void buck_regulator_setup(int en)
{
	if (en) {
		set_power_source(pwr_buck);
		CRM->VREG_CNTLbits.BUCK_CLKDIV = 0xf;
		CRM->VREG_CNTLbits.BUCK_SYNC_REC_EN = 1;
		CRM->VREG_CNTLbits.BUCK_EN = 1;
		while (!CRM->STATUSbits.VREG_BUCK_RDY)
			continue;
		CRM->VREG_CNTLbits.VREG_1P5V_SEL = 3;
		CRM->VREG_CNTLbits.VREG_1P5V_EN = 3;
		CRM->VREG_CNTLbits.VREG_1P8V_EN = 1;
		while (!CRM->STATUSbits.VREG_1P5V_RDY ||
			!CRM->STATUSbits.VREG_1P8V_RDY)
			continue;
	}
}

void buck_regulator_mode(int mod)
{
	/* Bypass mode */
	if (mod) {
		CRM->VREG_CNTLbits.BUCK_CLKDIV = 0xf;
		CRM->VREG_CNTLbits.VREG_1P8V_EN = 0;
		CRM->VREG_CNTLbits.VREG_1P5V_SEL = 3;
		CRM->VREG_CNTLbits.VREG_1P5V_EN = 3;
		CRM->VREG_CNTLbits.BUCK_BYPASS_EN = 1;
		CRM->VREG_CNTLbits.BUCK_SYNC_REC_EN = 0;
		CRM->VREG_CNTLbits.BUCK_EN = 1;
	}
}
