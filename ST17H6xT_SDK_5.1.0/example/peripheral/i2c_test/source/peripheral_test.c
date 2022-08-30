/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       peripheral_test.c
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
#include "pwm.h"
#include "peripheral_test.h"
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

static uint8 peripheral_TaskID;

extern void i2c_test(void);
void Peripheral_Init( uint8 task_id )
{		
    peripheral_TaskID = task_id;
    LOG("i2c test\n");
	i2c_test();
}

uint16 Peripheral_ProcessEvent( uint8 task_id, uint16 events )
{
    if (events & TIMER_1000_MS_EVT )
    {
        return (events ^ TIMER_1000_MS_EVT);
    }

    return 0;
}