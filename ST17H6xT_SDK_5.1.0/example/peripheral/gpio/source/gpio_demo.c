/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       gpio_demo.c
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
#include "gpio_demo.h"
#include "jump_function.h"
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
uint8 application_TaskID; 

volatile unsigned int int_flag = 0;

extern uint32_t s_gpio_wakeup_src[2];
void pos_cb(gpio_pin_e pin,gpio_polarity_e type)
{
	int_flag++;

	{
		LOG("int:%d\n",int_flag);
	}
}

void neg_cb(gpio_pin_e pin,gpio_polarity_e type)
{
	int_flag++;

	{
		LOG("int:%d\n",int_flag);
	}
}

gpioin_t pin_test[2];
void gpio_Init( uint8 task_id )
{
	volatile int ret;
	gpio_pin_e pin_i = P16;
	gpio_pin_e pin_o = P15;
	gpio_pupd_e type = GPIO_PULL_UP_S;
	
	application_TaskID = task_id;
	LOG("GPIO_demo\n");

	ret = gpioin_init(pin_test,sizeof(pin_test)/sizeof(pin_test[0]));
	if(ret != PPlus_SUCCESS)
	{
		LOG("gpio init error:%d\n",ret);
	}
	gpio_dir(pin_i, IE);
	gpio_pull_set(pin_i,type);
	ret = gpioin_register(pin_i,pos_cb,neg_cb);
	
	gpio_dir(pin_o,GPIO_OUTPUT);
	gpio_fast_write(pin_o,1);
	gpio_retention(pin_o,TRUE);
	
	osal_start_timerEx(application_TaskID, OSAL_ONCE_TIMER_EVT, 5000);
	osal_start_reload_timer(application_TaskID, OSAL_RELOAY_TIMER_EVT, 5000);
}

uint16 gpio_ProcessEvent( uint8 task_id, uint16 events )
{
    if(task_id != application_TaskID)
    {
        return 0;
    }
	
	if ( events & OSAL_ONCE_TIMER_EVT )
	{
		osal_start_timerEx(application_TaskID, OSAL_ONCE_TIMER_EVT, 5000);
		return ( events ^ OSAL_ONCE_TIMER_EVT );
	}

	if ( events & OSAL_RELOAY_TIMER_EVT )
	{
		return ( events ^ OSAL_RELOAY_TIMER_EVT );
	}

	return 0;
}
