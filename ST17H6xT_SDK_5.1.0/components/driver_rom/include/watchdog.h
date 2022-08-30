/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/
#ifndef __WATCHDOG_ROM_H__
#define __WATCHDOG_ROM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "bus_dev.h"

typedef enum
{
    WDG_2S =   0,
    WDG_4S =   1,
    WDG_8S =   2,
    WDG_16S =  3,
    WDG_32S =  4,
    WDG_64S =  5,
    WDG_128S = 6,
    WDG_256S = 7
}WDG_CYCLE_Type_e;

//when use watchdog,please umount its task and event,Watchdog_Init and Watchdog_ProcessEvent.
void Watchdog_Init(uint8 task_id);
uint16 Watchdog_ProcessEvent(uint8 task_id, uint16 events);

//watchdog default config:
//WDG_CYCLE_Type_e r_wdg_cycle = WDG_2S;
//bool r_wdg_int_mode = FALSE;
//uint32_t wdg_ms_cycle = 1000ms
//
//if you want change,please modify the following before watchdog start.
//for example:
//r_wdg_cycle = WDG_4S;
//r_wdg_int_mode = TRUE;

//if you want change feed watchdog cycle,please change wdg_ms_cycle.
//for example:
//wdg_ms_cycle = 1500;

//in most case,just use default setting is enough.

bool watchdog_init(WDG_CYCLE_Type_e cycle,bool int_mode);
void watchdog_feed(void);

#ifdef __cplusplus
}
#endif

#endif
