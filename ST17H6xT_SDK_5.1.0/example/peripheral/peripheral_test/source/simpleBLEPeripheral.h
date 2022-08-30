/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       simpleBLEperipheral.h
  Revised:         
  Revision:        

  Description:    This file contains the Simple BLE Peripheral sample application
                  definitions and prototypes.

 
**************************************************************************************************/

#ifndef SIMPLEBLEPERIPHERAL_H
#define SIMPLEBLEPERIPHERAL_H

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

#define OSAL_ONCE_TIMER_EVT                           0x0001
#define OSAL_RELOAY_TIMER_EVT                         0x0002


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the BLE Application
 */
extern void GPIO_Init( uint8 task_id );

/*
 * Task Event Processor for the BLE Application
 */
extern uint16 GPIO_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEBLEPERIPHERAL_H */
