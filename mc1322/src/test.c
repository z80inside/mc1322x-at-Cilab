#include "mc1322x.h"
#include "board-conf.h"
#include "config.h"
#include "rtc.h"
#include "gpio-util.h"
//#include "crm.h"
#include "Crm.h"
#include "Delay.h"

uint16_t uip_chksum(uint16_t data[], uint8_t len);

uint16_t ipblock[] = {0x0100, 0xf203, 0xf4f5, 0xf6f7};

uint16_t chksum;

#if 0
void sleep()
{
	*CRM_WU_CNTL = 0x1; /* enable wakeup from wakeup timer */
	*CRM_WU_TIMEOUT = 600000;
	*CRM_SLEEP_CNTL = 0x52; /* Doze: retain state, RAM pages 0, 1 */
	while((*CRM_STATUS & 0x1) == 0) { continue; }
	*CRM_STATUS = 1; 
	while((*CRM_STATUS & 0x1) == 0) { continue; }
	*CRM_STATUS = 1; 
}
#endif

int main(void)
{
	buck_regulator_setup(1);
	rtc_init_osc(0);
	rtc_calibrate();
	vreg_init();
	/*
	crmWuCtrl_t wu_settings;
	crmTimerWuCtrl_t tm_settings;
	crmSleepCtrl_t sl_settings;
	*/
	/*
	crmVRegCntl_t vreg_settings;
	crmBuckCntl_t buck_settings;
	crmVReg1P5VCntl_t p5v_settings;

	p5v_settings.vReg1P5VEn = 3;
	p5v_settings.vReg1P5VISel = 3;
	buck_settings.buckEn = 1;
	buck_settings.buckSyncRecEn = 1;
	buck_settings.buckBypassEn = 0;
	buck_settings.buckClkDiv = 0xf;
	vreg_settings.vReg = gBuckVReg_c;
	vreg_settings.cntl.buckCntl = buck_settings;
	vreg_settings.cntl.vReg1P5VCntl = p5v_settings;
	vreg_settings.cntl.vReg1P8VEn = 0;

	CRM_SetPowerSource(gCrmPwSBuckRegulation_c);
	CRM_VRegCntl(&vreg_settings);
	*/

	uart_init(INC, MOD, SAMP);

	gpio_set_pad_dir(44, 1);
	gpio_set_pad_dir(4, 1);

	gpio_set(4);

/*
	sl_settings.sleepType = 1;
	sl_settings.ramRet = 1;
	sl_settings.mcuRet = 1;
	sl_settings.digPadRet = 0;
	sl_settings.pfToDoBeforeSleep = NULL;

	tm_settings.timerWuEn = 1;
	tm_settings.timerWuIntEn = 0;
	tm_settings. timeOut = 600000;
	wu_settings.wuSource = gTimerWu_c;
	wu_settings.ctrl.timer = tm_settings;
	CRM_WuCntl(&wu_settings);

	CRM_BusStealingCntl(gManualBs_c, 1, 40);
*/
	while(1) {
		gpio_set(44);
		DelayMs(100);
//		gpio_reset(44);
//		CRM_GoToSleep(&sl_settings);
//		gpio_set(44);
		gpio_reset(44);
		DelayMs(100);
//		CRM_GoToSleep(&sl_settings);
		//chksum = uip_chksum(ipblock, 8);
	}
}
