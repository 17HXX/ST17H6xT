/**************************************************************************************************

  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       simpleBLEPeripheral.c
  Revised:        
  Revision:       

  Description:    This file contains the Simple BLE Peripheral sample application
                  

**************************************************************************************************/
/*********************************************************************
 * INCLUDES
 */
#include "rom_sym_def.h"
#include "bcomdef.h"
#include "rf_phy_driver.h"
#include "global_config.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "gatt.h"
#include "hci.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "devinfoservice.h"
#include "peripheral.h"
#include "gapbondmgr.h"
#include "simpleBLEPeripheral.h"
#include "simpleGATTprofile.h"
#include "hci_tl.h"
/*********************************************************************
 * MACROS
 */
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
uint8 application_TaskID;   // Task ID for internal task/event processing

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void simpleBLEPeripheral_ProcessOSALMsg( osal_event_hdr_t *pMsg );

/*********************************************************************
 * PROFILE CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleBLEPeripheral_Init
 *
 * @brief   Initialization function for the Simple BLE Peripheral App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void SimpleBLEPeripheral_Init( uint8 task_id )
{
	application_TaskID = task_id;
	#if( defined( SERV_GGS ) && SERV_GGS )
	    GGS_AddService( );            // GAP
	#endif
	#if( defined( SERV_GATT_SERV ) && SERV_GATT_SERV )
	    GATTServApp_AddService( );    // GATT attributes
    #endif
	#if( defined( SERV_DEV_INFO ) && SERV_DEV_INFO )
	    DevInfo_AddService();                           // Device Information Service
    #endif
	simpleGATTProfile_AddService();					// simple GATT Profile Service

    // Setup a delayed profile startup
    osal_set_event( application_TaskID, SBP_START_DEVICE_EVT );
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_ProcessEvent
 *
 * @brief   Simple BLE Peripheral Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
//static int s_tmp_cnt;
uint16 SimpleBLEPeripheral_ProcessEvent( uint8 task_id, uint16 events )
{
    VOID task_id; // OSAL required parameter that isn't used in this function
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif

    if ( events & SYS_EVENT_MSG )
    {
        uint8 *pMsg;

        if ( (pMsg = osal_msg_receive( application_TaskID )) != NULL )
        {
            simpleBLEPeripheral_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );

            // Release the OSAL message
            osal_msg_deallocate( pMsg );
        }

        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if ( events & SBP_START_DEVICE_EVT )
    {
        // Start the Device
        GAPRole_StartDevice( );
        // Start Bond Manager
        GAPBondMgr_Register( NULL );
        return ( events ^ SBP_START_DEVICE_EVT );
    }
    return 0;
}

/*********************************************************************
 * @fn      simpleBLEPeripheral_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void simpleBLEPeripheral_ProcessOSALMsg( osal_event_hdr_t *pMsg )
{
    #ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
    switch ( pMsg->event )
    {  
    	#ifdef _PHY_DEBUG 
			case GAP_MSG_EVENT:
				LOG("	Peripheral Bypass Message to Application\n");
				LOG("	Message Opcode 0x%X\n",((gapEventHdr_t *)pMsg)->opcode);
				if( GAP_DEVICE_INIT_DONE_EVENT == ((gapEventHdr_t *)pMsg)->opcode )
				{
					LOG("	Device Initiliaztion Done\n");
					LOG("	Please Start Application Logic...\n");
				}
			break;
		#endif

      }
}

/*********************************************************************
*********************************************************************/
