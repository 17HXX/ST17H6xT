/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/
#include "rom_sym_def.h"
#include "watchdog.h"
#include "error.h"
#include "clock.h"
#include "jump_function.h"

extern volatile uint8 g_clk32K_config;
extern uint32_t s_config_swClk1;
#if(CFG_WDT_ENABLE==1)
WDG_CYCLE_Type_e g_wdt_cycle = 0xFF;//valid value:0~7.0xFF:watchdog disable.
#endif


#if(CFG_WDT_ENABLE==1)
//void hal_watchdog_init(void)
//{
//	watchdog_init(g_wdt_cycle,/*int_mode*/0);//wdt polling_mode
//	s_config_swClk1|=_CLK_WDT;
//}
void hal_watchdog_init(void)
{
//	watchdog_init(g_wdt_cycle,/*int_mode*/0);//wdt polling_mode
//	s_config_swClk1|=_CLK_WDT;
	// bugfix for watchdog start osal timer when osal memory not init
	extern uint32 wdg_ms_cycle;
	extern uint8 watchdog_TaskID;
	wdg_ms_cycle = 0xFFF0;
	watchdog_TaskID = 0xF0;
	watchdog_init(g_wdt_cycle,/*int_mode*/0);//wdt polling_mode
	s_config_swClk1|=_CLK_WDT;
	
	extern void watchdog_sleep_handler(void);
	watchdog_sleep_handler();	
}
#endif

int hal_watchdog_config(WDG_CYCLE_Type_e cycle)
{
	
#if(CFG_WDT_ENABLE==1)
    if(cycle > WDG_256S)
        return PPlus_ERR_INVALID_PARAM;
    else
        g_wdt_cycle = cycle;

    hal_watchdog_init();
    JUMP_FUNCTION_SET(HAL_WATCHDOG_INIT, (uint32_t)&hal_watchdog_init);
	return PPlus_SUCCESS;
#else
	(void) cycle;
	return PPlus_ERR_UNINITIALIZED;
#endif	
    
}
