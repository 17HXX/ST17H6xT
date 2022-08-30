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
const uint8_t pin_map[GPIO_NUM] = 
{
	0,    //p0
	1,    //p1
	2,    //p2
	3,    //p3
	7,    //p7
	9,    //p9
	10,   //p10
	11,   //p11
	14,   //p14
	15,   //p15
	16,   //p16
	17,   //p17
	18,   //p18
	20,   //p20
	23,   //p23
	24,   //p24
	25,   //p25
	26,   //p26
	27,   //p27
	31,   //p31
	32,   //p32
	33,   //p33
	34,   //p34
};

extern uint32_t s_gpio_wakeup_src[2];
void pos_cb(gpio_pin_e pin,gpio_polarity_e type)
{
	uint8_t pin_idx = pin_map[pin];
	uint32_t triggered = s_gpio_wakeup_src[pin_idx/32] & BIT(pin_idx%32);
	
	LOG("pin:%d (pos) 0x%x 0x%x ",pin,s_gpio_wakeup_src[0],s_gpio_wakeup_src[1]);
	int_flag++;

	if(triggered)
	{
		LOG("wakeup:%d\n",int_flag);
	}
	else
	{
		LOG("int:%d\n",int_flag);
	}
}

void neg_cb(gpio_pin_e pin,gpio_polarity_e type)
{
	uint8_t pin_idx = pin_map[pin];
	uint32_t triggered = s_gpio_wakeup_src[pin_idx/32] & BIT(pin_idx%32);

	LOG("pin:%d (neg) 0x%x 0x%x ",pin,s_gpio_wakeup_src[0],s_gpio_wakeup_src[1]);
	int_flag++;

	if(triggered)
	{
		LOG("wakeup:%d\n",int_flag);
	}
	else
	{
		LOG("int:%d\n",int_flag);
	}
}

gpioin_t pin_test[2];
void gpio_Init( uint8 task_id )
{
	volatile int ret;
	gpio_pin_e pin_i = P14;
	gpio_pin_e pin_o = P15;
	gpio_pupd_e type = GPIO_PULL_DOWN;
	
	application_TaskID = task_id;
	LOG("GPIO_demo\n");

	ret = gpioin_init(pin_test,sizeof(pin_test)/sizeof(pin_test[0]));
	if(ret != PPlus_SUCCESS)
	{
		LOG("gpio init error:%d\n",ret);
	}
	
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
