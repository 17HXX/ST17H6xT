/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       OSAL_bugfix.c
  Revised:         
  Revision:       

  Description:    This file contains function that allows user setup tasks



**************************************************************************************************/

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "rom_sym_def.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"

/* LL */
#include "ll.h"

/* HCI */
#include "hci_tl.h"

#if defined ( OSAL_CBTIMER_NUM_TASKS )
  #include "osal_cbTimer.h"
#endif

/* L2CAP */
#include "l2cap.h"

/* gap */
#include "gap.h"
#include "gapgattserver.h"
#include "gapbondmgr.h"

/* GATT */
#include "gatt.h"

#include "gattservapp.h"

/* Profiles */
#include "peripheral.h"

/* Application */
#include "bugfix.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */
// The order in this table must be identical to the task initialization calls below in osalInitTask.
pTaskEventHandlerFn tasksArr[] =
{
  LL_ProcessEvent,                                                  // task 0
  bugfix_ProcessEvent                                  // task 1
};

uint16 tasksCnt = sizeof( tasksArr ) / sizeof( tasksArr[0] );
uint16 *tasksEvents;

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osalInitTasks
 *
 * @brief   This function invokes the initialization function for each task.
 *
 * @param   void
 *
 * @return  none
 */
void osalInitTasks( void )
{
  uint8 taskID = 0;

  tasksEvents = (uint16 *)osal_mem_alloc( sizeof( uint16 ) * tasksCnt);
  osal_memset( tasksEvents, 0, (sizeof( uint16 ) * tasksCnt));

  /* LL Task */
  LL_Init( taskID++ );

  bugfix_Init( taskID );
}
/*********************************************************************
*********************************************************************/
