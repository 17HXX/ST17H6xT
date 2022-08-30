/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/
#include "rom_sym_def.h"
#include "types.h"
#include "ll_sleep.h"
#include "bus_dev.h"
#include "string.h"

#include "pwrmgr.h"
#include "error.h"
#include "gpio.h"
#include "log.h"
#include "clock.h"
#include "jump_function.h"

extern uint32_t s_config_swClk0;
extern uint32_t s_config_swClk1;

/*
    osal_idle_task will be call
*/

void osal_idle_task (void)
{
    AP_WDT_FEED;
    osal_pwrmgr_powerconserve0();
}
int hal_pwrmgr_init(void)
{
	
#if(CFG_SLEEP_MODE == PWR_MODE_NO_SLEEP)
	pwrmgr_config(PWR_MODE_NO_SLEEP);
#else
	pwrmgr_config(PWR_MODE_SLEEP);
#endif

    pwrmgr_init();
	
    s_config_swClk0 = DEF_CLKG_CONFIG_0;
    s_config_swClk1 = DEF_CLKG_CONFIG_1;
#if(CFG_WDT_ENABLE==1)
    /*
        if wdt enable, set osal idle task to feed wdt before powerconserve
    */
    if(AP_WDT_ENABLE_STATE)
        JUMP_FUNCTION_SET(OSAL_POWER_CONSERVE,(uint32_t)&osal_idle_task);
#endif

    return PPlus_SUCCESS;
}

void pwrmgr_poweroff(pwroff_cfg_t *pcfg, uint8_t wakeup_pin_num) // check
{
    drv_disable_irq();
    subWriteReg(0x4000f01c, 6, 6, 0x00); // disable software control
    //(void)(wakeup_pin_num);

    for (uint8_t i = 0; i < wakeup_pin_num; i++)
    {
        if (pcfg[i].type == POL_FALLING)
            gpio_pull_set(pcfg[i].pin, GPIO_PULL_UP_S);
        else
            gpio_pull_set(pcfg[i].pin, GPIO_PULL_DOWN);

        gpio_wakeup_set(pcfg[i].pin, pcfg[i].type);
    }
    /**
        config reset casue as RSTC_OFF_MODE
        reset path walkaround dwc
    */
    AP_AON->SLEEP_R[0] = 4;
    enter_sleep_off_mode(SYSTEM_OFF_MODE);

    while (1)
        ;
}
