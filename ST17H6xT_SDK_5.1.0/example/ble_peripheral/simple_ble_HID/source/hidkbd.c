/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/

/******************************************************************************


 *****************************************************************************/


/*********************************************************************
    INCLUDES
*/
#include "rom_sym_def.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "gatt.h"
#include "hci.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "gatt_profile_uuid.h"
#include "linkdb.h"
#include "peripheral.h"
#include "gapbondmgr.h"
#include "peripheral.h"
#include "hidkbdservice.h"
#include "hiddev.h"
#include "global_config.h"
#include "hidkbd.h"
#include "hci_tl.h"
#include "ll.h"
#include "ll_common.h"
#include "ll_def.h"
#include "OSAL_Memory.h"
#include "log.h"


/*********************************************************************
    MACROS
*/
// Selected HID keycodes
#define KEY_RIGHT_ARROW             0x4F
#define KEY_LEFT_ARROW              0x50
#define KEY_NONE                    0x00

// Selected HID LED bitmaps
#define LED_NUM_LOCK                0x01
#define LED_CAPS_LOCK               0x02

// Selected HID mouse button values
#define MOUSE_BUTTON_1              0x01
#define MOUSE_BUTTON_NONE           0x00

// HID keyboard input report length
#define HID_KEYBOARD_IN_RPT_LEN     8

// HID LED output report length
#define HID_LED_OUT_RPT_LEN         1

// HID mouse input report length
#define HID_MOUSE_IN_RPT_LEN        5

#if EN_CONSUMER_MODE
    // HID consumer control input report length
    #define HID_CC_IN_RPT_LEN                     1//2//1

#endif
/*********************************************************************
    CONSTANTS
*/

// HID idle timeout in msec; set to zero to disable timeout
#define DEFAULT_HID_IDLE_TIMEOUT              0

// Battery level is critical when it is less than this %
#define DEFAULT_BATT_CRITICAL_LEVEL           6

/*********************************************************************
    TYPEDEFS
*/

/*********************************************************************
    GLOBAL VARIABLES
*/

// Task ID
uint8 application_TaskID;

/*********************************************************************
    EXTERNAL VARIABLES
*/


/*********************************************************************
    EXTERNAL FUNCTIONS
*/

/*********************************************************************
    LOCAL VARIABLES
*/
#if( defined( EN_KEYBOARD_INPUT ) && EN_KEYBOARD_INPUT)
// HID Dev configuration
static hidDevCfg_t hidKbdCfg =
{
    DEFAULT_HID_IDLE_TIMEOUT,   // Idle timeout
    HID_KBD_FLAGS               // HID feature flags
};
#endif


/*********************************************************************
    LOCAL FUNCTIONS
*/
static void hidKbd_ProcessOSALMsg( osal_event_hdr_t* pMsg );
static void hidKbd_ProcessGattMsg( gattMsgEvent_t* pMsg );
#if( defined( EN_KEYBOARD_INPUT ) && EN_KEYBOARD_INPUT)
void hidKbdSendReport( uint8 keycode );
void hidKbdSendMouseReport( uint8 buttons,uint8 x,uint8 y );
static uint8 hidKbdRcvReport( uint8 len, uint8* pData );
static uint8 hidKbdRptCB( uint8 id, uint8 type, uint16 uuid,
                          uint8 oper, uint16* pLen, uint8* pData );
static void hidKbdEvtCB( uint8 evt );
#endif
#if( defined( EN_CONSUMER_MODE ) && EN_CONSUMER_MODE)
    static void hidCCSendReport( uint8 cmd, bool keyPressed, uint8 keyRepeated );
    void hidCCSendReportKey( uint8 cmd, bool keyPressed);
#endif

/*********************************************************************
    PROFILE CALLBACKS
*/

/*********************************************************************
    PUBLIC FUNCTIONS
*/

/*********************************************************************
    @fn      HidEmuKbd_Init

    @brief   Initialization function for the HidEmuKbd App Task.
            This is called during initialization and should contain
            any application specific initialization (ie. hardware
            initialization/setup, table initialization, power up
            notificaiton ... ).

    @param   task_id - the ID assigned by OSAL.  This ID should be
                      used to send messages and set timers.

    @return  none
*/
void HidKbd_Init( uint8 task_id )
{
    application_TaskID = task_id;
    #ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif

    // Set up HID keyboard service
    HidKbd_AddService();
    osal_set_event( application_TaskID, START_DEVICE_EVT );
//	GAP_RegisterForHCIMsgs(application_TaskID);
}

/*********************************************************************
    @fn      HidEmuKbd_ProcessEvent

    @brief   HidEmuKbd Application Task event processor.  This function
            is called to process all events for the task.  Events
            include timers, messages and any other user defined events.

    @param   task_id  - The OSAL assigned task ID.
    @param   events - events to process.  This is a bit map and can
                     contain more than one event.

    @return  events not processed
*/
uint16 HidKbd_ProcessEvent( uint8 task_id, uint16 events )
{
    VOID task_id; // OSAL required parameter that isn't used in this function
//    #ifdef _PHY_DEBUG 
//		LOG("%s,%s,Line %d,events 0x%X\n",__FILE__,__func__,__LINE__,events);
//	#endif

    if ( events & SYS_EVENT_MSG )
    {
        uint8* pMsg;

        if ( (pMsg = osal_msg_receive( application_TaskID )) != NULL )
        {
            hidKbd_ProcessOSALMsg( (osal_event_hdr_t*)pMsg );
            // Release the OSAL message
            osal_msg_deallocate( pMsg );
        }

        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if ( events & START_DEVICE_EVT )
    {
//		osal_start_reload_timer(application_TaskID,HID_TEST_EVT,2000);
		return ( events ^ START_DEVICE_EVT );
    }

	if ( events & HID_TEST_EVT )
    {
		#if( defined(EN_CONSUMER_MODE) && EN_CONSUMER_MODE )
//	    	#ifdef _PHY_DEBUG 
//				LOG("HID_TEST_EVT\n");
//			#endif
			hidCCSendReport(0,1,0);
	        hidCCSendReport(0,0,0);
		#endif
        return ( events ^ HID_TEST_EVT );
    }

    return 0;
}

/*********************************************************************
    @fn      hidKbd_ProcessOSALMsg

    @brief   Process an incoming task message.

    @param   pMsg - message to process

    @return  none
*/
static void hidKbd_ProcessOSALMsg( osal_event_hdr_t* pMsg )
{
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d,pMsg->event 0x%X\n",__FILE__,__func__,__LINE__,pMsg->event);
	#endif

    switch ( pMsg->event )
    {
    case GATT_MSG_EVENT:
        hidKbd_ProcessGattMsg( (gattMsgEvent_t*)pMsg );
        break;
	case HCI_GAP_EVENT_EVENT:
    {
        switch( pMsg->status )
        {
	        case HCI_COMMAND_COMPLETE_EVENT_CODE:
			{
				#ifdef _PHY_DEBUG 
	            hciEvt_CmdComplete_t* pHciMsg;
				pHciMsg	= (hciEvt_CmdComplete_t*)pMsg;
	            LOG("	HCI_COMMAND_COMPLETE_EVENT_CODE: %x\n", pHciMsg->cmdOpcode);
	            //safeToDealloc = gapProcessHCICmdCompleteEvt( (hciEvt_CmdComplete_t *)pMsg );
	            #endif
			
			}
	            break;

	        default:
	            break;
        }
    }
	break;

    default:
        break;
    }
}


/*********************************************************************
    @fn      hidKbd_ProcessGattMsg

    @brief   Process GATT messages

    @return  none
*/
static void hidKbd_ProcessGattMsg( gattMsgEvent_t* pMsg )
{
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
}

#if( defined( EN_KEYBOARD_INPUT ) && EN_KEYBOARD_INPUT)
/*********************************************************************
    @fn      hidKbdSendReport

    @brief   Build and send a HID keyboard report.

    @param   keycode - HID keycode.

    @return  none
*/
void hidKbdSendReport( uint8 keycode )
{
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
    uint8 buf[HID_KEYBOARD_IN_RPT_LEN];
    buf[0] = 0;         // Modifier keys
    buf[1] = 0;         // Reserved
    buf[2] = keycode;   // Keycode 1
    buf[3] = 0;         // Keycode 2
    buf[4] = 0;         // Keycode 3
    buf[5] = 0;         // Keycode 4
    buf[6] = 0;         // Keycode 5
    buf[7] = 0;         // Keycode 6
    HidDev_Report( HID_RPT_ID_KEY_IN, HID_REPORT_TYPE_INPUT,
                   HID_KEYBOARD_IN_RPT_LEN, buf );
}

/*********************************************************************
    @fn      hidKbdSendMouseReport

    @brief   Build and send a HID mouse report.

    @param   buttons - Mouse button code

    @return  none
*/
void hidKbdSendMouseReport( uint8 buttons,uint8 x,uint8 y )
{
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
    uint8 buf[HID_MOUSE_IN_RPT_LEN];
    buf[0] = buttons;   // Buttons
    buf[1] = x;         // X
    buf[2] = y;         // Y
    buf[3] = 0;         // Wheel
    buf[4] = 0;         // AC Pan
    HidDev_Report( HID_RPT_ID_MOUSE_IN, HID_REPORT_TYPE_INPUT,
                   HID_MOUSE_IN_RPT_LEN, buf );
}

uint8 hidKbdSendVoiceCMDtReport( uint8 keycode )
{
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
    uint8 status;
    return status;
}


/*********************************************************************
    @fn      hidKbdRcvReport

    @brief   Process an incoming HID keyboard report.

    @param   len - Length of report.
    @param   pData - Report data.

    @return  status
*/
static uint8 hidKbdRcvReport( uint8 len, uint8* pData )
{
    // verify data length
    if ( len == HID_LED_OUT_RPT_LEN )
    {
        // set keyfob LEDs
        //HalLedSet( HAL_LED_1, ((*pData & LED_CAPS_LOCK) == LED_CAPS_LOCK) );
        //HalLedSet( HAL_LED_2, ((*pData & LED_NUM_LOCK) == LED_NUM_LOCK) );
        return SUCCESS;
    }
    else
    {
        return ATT_ERR_INVALID_VALUE_SIZE;
    }
}

/*********************************************************************
    @fn      hidKbdRptCB

    @brief   HID Dev report callback.

    @param   id - HID report ID.
    @param   type - HID report type.
    @param   uuid - attribute uuid.
    @param   oper - operation:  read, write, etc.
    @param   len - Length of report.
    @param   pData - Report data.

    @return  GATT status code.
*/
static uint8 hidKbdRptCB( uint8 id, uint8 type, uint16 uuid,
                          uint8 oper, uint16* pLen, uint8* pData )
{
    uint8 status = SUCCESS;
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif

    return status;
}

/*********************************************************************
    @fn      hidKbdEvtCB

    @brief   HID Dev event callback.

    @param   evt - event ID.

    @return  HID response code.
*/
static void hidKbdEvtCB( uint8 evt )
{
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
    // process enter/exit suspend or enter/exit boot mode
    return;
}
#endif

#if( defined( EN_CONSUMER_MODE ) && EN_CONSUMER_MODE)
/*********************************************************************
  @fn	   hidCCSendReport

  @brief   Build and send a HID Consumer Control report.

  @param   cmd - bitmap of left/middle/right mouse button states
  @param   keyPressed - amount of mouse movement along X-axis in units of Mickeys (1/200 of an inch)
  @param   keyRepeated - amount of mouse movement along Y-axis

  @return  none
*/
static void hidCCSendReport( uint8 cmd, bool keyPressed, uint8 keyRepeated )
{
  // Only send the report if something meaningful to report
  if ( keyRepeated == 0 )
  {
	  uint8 buf[HID_CC_IN_RPT_LEN] = {0};

	  // No need to include Report Id
	  if ( keyPressed )
	  {
		  buf[0]=0X20;
	 
	  }
	  else
	  {
		  buf[0]=0X00;

	  }

	  HidDev_Report( HID_RPT_ID_CC_IN, HID_REPORT_TYPE_INPUT,
					 HID_CC_IN_RPT_LEN, buf );
  }
}
void hidCCSendReportKey( uint8 cmd, bool keyPressed)
{
  uint8 buf[1] = {0};

  if(keyPressed==1)
  {
	  buf[0]=cmd;
  }
  else
	  buf[0]=0;

  HidDev_Report( HID_RPT_ID_CC_IN, HID_REPORT_TYPE_INPUT,
				 1, buf );
}
#endif
/*********************************************************************

*********************************************************************/
