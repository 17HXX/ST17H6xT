/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       peripheral_test.h
  Revised:         
  Revision:        

  Description:    

 
**************************************************************************************************/

#ifndef __PERIPHERAL_TEST_H__
#define __PERIPHERAL_TEST_H__

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

#define TIMER_1000_MS_EVT                                0x0001

extern void Peripheral_Init( uint8 task_id );

extern uint16 Peripheral_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif 
