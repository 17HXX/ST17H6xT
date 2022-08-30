/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       gpio_demo.h
  Revised:         
  Revision:        

  Description:    

 
**************************************************************************************************/

#ifndef __GPIO_DEMO_H__
#define __GPIO_DEMO_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

#define GPIO_WAKEUP_PIN_NUM     							2

#define OSAL_ONCE_TIMER_EVT                           		0x0001
#define OSAL_RELOAY_TIMER_EVT                         		0x0002

extern void gpio_Init( uint8 task_id );
extern uint16 gpio_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif 
