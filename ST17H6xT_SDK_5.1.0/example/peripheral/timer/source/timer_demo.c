/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       timer_demo.c
  Revised:        
  Revision:       

  Description:  
                  

**************************************************************************************************/
/*********************************************************************
 * INCLUDES
 */
#include "rom_sym_def.h"
#include "global_config.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "jump_function.h"
#include "timer.h"
#include "timer_demo.h"
#include  "log.h"
/*********************************************************************
 * MACROS
 */
//#define LOG(...)  
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************a************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static uint8 timer_TaskID;
static uint8_t s_testCase = 0;
static void timer_test(uint8_t testCase);

void timer_int_process(uint8_t evt)
{
    switch(evt)
    {
    case HAL_EVT_TIMER_4:
        LOG("t4\n");
        break;

    case HAL_EVT_WAKEUP:
        LOG("wakeup\n");
        //LOG("timer will disable when sleep,so if you want it work please init it when wakeup\n");
        break;

    case HAL_EVT_SLEEP:
        LOG("sleep\n");
        break;

    default:
        LOG("err ");
        break;
    }
}

void timer_Init( uint8 task_id )
{
    timer_TaskID = task_id;
    LOG("timer demo\n");
		
    _symrom_osal_start_reload_timer( timer_TaskID, TIMER_1000_MS_EVT, 1000);
}

static void timer_test(uint8_t testCase)
{
    switch(testCase)
    {
    case 0:
        LOG("\ninit timer4\n");
        hal_timer_init(timer_int_process);
		hal_timer_set(AP_TIMER_ID_4,1000000);
        break;

    case 1:
        LOG("\nmask timer4 init\n");
        hal_timer_mask_int(AP_TIMER_ID_4,1);
        break;

    case 2:
        LOG("\nunmask timer4 init\n");
        hal_timer_mask_int(AP_TIMER_ID_4,0);
        break;

    case 3:
        LOG("\nstop timer4 int\n");
        hal_timer_stop(AP_TIMER_ID_4);
		hal_timer_deinit();
        break;

    default:
        break;
    }
}

uint16 timer_ProcessEvent( uint8 task_id, uint16 events )
{
    static uint8_t min_count = 4;

    if (events & TIMER_1000_MS_EVT )
    {
		min_count++;
        if(min_count == 5)
        {
            timer_test(s_testCase);
            min_count = 0;

            if(s_testCase < 3)
                s_testCase++;
            else
                s_testCase = 0;
        }
		
		//LOG("osal_timer\n");
        return (events ^ TIMER_1000_MS_EVT);
    }

    return 0;
}
