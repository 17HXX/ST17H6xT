/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/
#ifndef __PWRMGR_ROM_H__
#define __PWRMGR_ROM_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "bus_dev.h"
#include "gpio.h"

#define PWR_MODE_NO_SLEEP           1
#define PWR_MODE_SLEEP              2
#define PWR_MODE_PWROFF_NO_SLEEP    4

//WAKEUP FROM STANDBY MODE
#define WAKEUP_PIN_MAX   3

#define HAL_PWRMGR_TASK_MAX_NUM     10


#define   RET_SRAM0         BIT(0)  /*32K, 0x1fff0000~0x1fff7fff*/


#define   DEF_CLKG_CONFIG_0       (_CLK_IOMUX|_CLK_UART0|_CLK_GPIO|_CLK_SPIF)

#define   DEF_CLKG_CONFIG_1       (_CLK_M0_CPU | _CLK_BB |_CLK_TIMER |_CLK_BBREG \
                                   |_CLK_TIMER1|_CLK_TIMER2|_CLK_TIMER3|_CLK_TIMER4|_CLK_COM)

typedef struct
{
    gpio_pin_e pin;
    gpio_polarity_e type;
    uint16_t on_time;
} pwroff_cfg_t;


extern uint32_t g_system_reset_cause;
extern uint32_t s_gpio_wakeup_src[2];

typedef void (*pwrmgr_Hdl_t)(void);

int pwrmgr_init(void);
bool pwrmgr_is_lock(MODULE_e mod);
int pwrmgr_lock(MODULE_e mod);
int pwrmgr_unlock(MODULE_e mod);
int pwrmgr_register(MODULE_e mod, pwrmgr_Hdl_t sleepHandle, pwrmgr_Hdl_t wakeupHandle);
int pwrmgr_unregister(MODULE_e mod);
int pwrmgr_wakeup_process(void) __attribute__((weak));
int pwrmgr_sleep_process(void) __attribute__((weak));
int pwrmgr_RAM_retention(uint32_t sram);
int pwrmgr_clk_gate_config(MODULE_e module);
int pwrmgr_RAM_retention_clr(void);
int pwrmgr_RAM_retention_set(void);
int pwrmgr_LowCurrentLdo_enable(void);
int pwrmgr_LowCurrentLdo_disable(void);

void pwrmgr_poweroff(pwroff_cfg_t* pcfg, uint8_t wakeup_pin_num);
void pwrmgr_enter_standby(pwroff_cfg_t* pcfg,uint8_t wakeup_pin_num) ;

#ifdef __cplusplus
}
#endif


#endif


