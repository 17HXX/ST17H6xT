/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       peripheral.c
  Revised:         
  Revision:        

  Description:    GAP Peripheral Role


**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "rom_sym_def.h"
#include "ll_common.h"
#include "bcomdef.h"
#include "OSAL.h"
#include "hci_tl.h"
#include "l2cap.h"
#include "gap.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "peripheral.h"
#include "gapbondmgr.h"
#include "gatt_profile_uuid.h"

/*********************************************************************
 * MACROS
 */
#define GAP_CONFIG_HID_ENABLE					BLE_HID
#define GAP_CONFIG_STATIC_ADDR					FALSE

#define GAP_PROFILE_ROLE						(GAP_PROFILE_PERIPHERAL | GAP_PROFILE_OBSERVER )
#if ( HOST_CONFIG & OBSERVER_CFG  )
#define DEFAULT_MAX_SCAN_RES					(20)
#else
#define DEFAULT_MAX_SCAN_RES					(0)
#endif
#if ( HOST_CONFIG & PERIPHERAL_CFG )
#define DEFAULT_DISCOVERABLE_MODE             	( GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED )
#define GAP_DEFAULT_ADV_TYPE					GAP_ADTYPE_ADV_IND
#define GAP_DEFAULT_ADV_ADDR_TYPE				ADDRTYPE_STATIC
#define GAP_DEFAULT_ADV_INIT_ADDR				{0xCC,0x01,0x02,0x03,0x04,0x05}
#define GAP_DEFAULT_ADV_CHNMAP					( GAP_ADVCHAN_37 | GAP_ADVCHAN_38 | GAP_ADVCHAN_39 )
#define GAP_DEFAULT_ADV_POLICY					GAP_FILTER_POLICY_ALL
#define GAP_DEFAULT_ADV_INTERVAL                (800)  //x 0.625ms

#define GAP_DEFAULT_ENABLE_SMP					FALSE

#define GAP_DEFAULT_ENABLE_PARAM_UPDATE			FALSE
#define GAP_DEFAULT_PARAM_UPDATE_PAUSE			6			// unit:s
#define PARAM_UPDATE_EVT						0x0008
#define GAP_DEFAULT_DESIRED_MIN_CONN_INTERVAL	80			// unit 1.25ms
#define GAP_DEFAULT_DESIRED_MAX_CONN_INTERVAL	80
#define GAP_DEFAULT_DESIRED_SLAVE_LATENCY		0
#define GAP_DEFAULT_DESIRED_CONN_TIMEOUT        500			// unit 10ms


#define GAP_DEFAULT_ENABLE_SEC_ADV				FALSE
#define GAP_DEFAULT_SEC_ADV_TYPE				GAP_ADTYPE_ADV_NONCONN_IND
#define GAP_DEFAULT_TERMINATE_REASON			HCI_ERROR_CODE_REMOTE_USER_TERM_CONN

#define GAP_CONFIG_PERIODIC_READ_RSSI			FALSE
#define RSSI_READ_EVT 							0x0001
#define GAP_DEFAULT_READ_RSSI_INTV				(5000)

#define GAP_CONFIG_ENABLE_DLE					FALSE
#define DLE_UPDATE_EVT 							0x0002
#define GAP_DEFAULT_START_DLE					(500)
#define GAP_DEFAULT_TX_OCTETS					251
#define GAP_DEFAULT_TX_TIME						2120

#define GAP_CONFIG_ENABLE_CHANGE_PHY			FALSE
#define PHY_UPDATE_EVT 							0x0004
#define GAP_CONFIG_PHY_1M						(0x01)
#define GAP_CONFIG_PHY_2M						(0x02)
#define GAP_DEFAULT_PRE_PHY						GAP_CONFIG_PHY_2M
#define GAP_DEFAULT_START_CHANGE_PHY			(1000)

#define GAP_DEFAULT_MSG_BYPASS_ENABLE			FALSE
#if(defined(GAP_DEFAULT_MSG_BYPASS_ENABLE) && GAP_DEFAULT_MSG_BYPASS_ENABLE)
extern uint8 application_TaskID;
#define GAP_DEFAULT_MSG_BYPASS_ID				(application_TaskID)
#else
#define GAP_DEFAULT_MSG_BYPASS_ID				(gapRole_TaskID)
#endif
#endif

/*********************************************************************
 * CONSTANTS
 */
// Profile Events
//#define START_CONN_UPDATE_EVT         0x0004  // Start Connection Update Procedure
//#define CONN_PARAM_TIMEOUT_EVT        0x0008  // Connection Parameters Update Timeout

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
#if(defined(GAP_CONFIG_STATIC_ADDR) && GAP_CONFIG_STATIC_ADDR)
uint8 BD_STATIC_ADDR[B_ADDR_LEN]={0x30,0x31,0x32,0x33,0x34,0xC1};
#endif

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8 gapRole_TaskID;   // Task ID for internal task/event processing

/*********************************************************************
 * Profile Parameters - reference GAPROLE_PROFILE_PARAMETERS for
 * descriptions
 */
uint16 gapRole_ConnectionHandle = INVALID_CONNHANDLE;


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void gapRole_ProcessOSALMsg( osal_event_hdr_t *pMsg );
static void gapRole_ProcessGAPMsg( gapEventHdr_t *pMsg );
static void gapRole_SetupGAP( void );

/*********************************************************************
 * @brief   Does the device initialization.
 *
 * Public function defined in peripheral.h.
 */
bStatus_t GAPRole_StartDevice( )
{
    // Start the GAP
    gapRole_SetupGAP();
    return ( SUCCESS );
}

/*********************************************************************
 * @brief   Task Initialization function.
 *
 * Internal function defined in peripheral.h.
 */
void GAPRole_Init( uint8 task_id )
{
	gapRole_TaskID = task_id;

	#if( defined(GAP_CONFIG_ENABLE_DLE) && GAP_CONFIG_ENABLE_DLE)
		llInitFeatureSetDLE(TRUE);
	#endif
	#if( defined(GAP_CONFIG_ENABLE_CHANGE_PHY) && GAP_CONFIG_ENABLE_CHANGE_PHY)
		llInitFeatureSet2MPHY(TRUE);
	#endif
	GAP_RegisterForHCIMsgs(gapRole_TaskID);
}

/*********************************************************************
 * @brief   Task Event Processor function.
 *
 * Internal function defined in peripheral.h.
 */
uint16 GAPRole_ProcessEvent( uint8 task_id, uint16 events )
{
	VOID task_id; // OSAL required parameter that isn't used in this function

	if ( events & SYS_EVENT_MSG )
	{
		uint8 *pMsg;

		if ( (pMsg = osal_msg_receive( gapRole_TaskID )) != NULL )
		{
			gapRole_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );

			// Release the OSAL message
			osal_msg_deallocate( pMsg );
		}
		return (events ^ SYS_EVENT_MSG);
	}
#if(defined(GAP_DEFAULT_MSG_BYPASS_ENABLE) && GAP_DEFAULT_MSG_BYPASS_ENABLE)

#else
	#if(defined(GAP_CONFIG_PERIODIC_READ_RSSI) && GAP_CONFIG_PERIODIC_READ_RSSI)
	if ( events & RSSI_READ_EVT )
	{
		if( gapRole_ConnectionHandle != INVALID_CONNHANDLE )
			HCI_ReadRssiCmd( gapRole_ConnectionHandle );
		return (events ^ RSSI_READ_EVT);		
	}
	#endif

	#if( defined(GAP_CONFIG_ENABLE_DLE) && GAP_CONFIG_ENABLE_DLE)
	if ( events & DLE_UPDATE_EVT )
	{
		#ifdef _PHY_DEBUG 
			LOG("DLE_UPDATE_EVT\n");
		#endif
		if( gapRole_ConnectionHandle != INVALID_CONNHANDLE )
			HCI_LE_SetDataLengthCmd(gapRole_ConnectionHandle,GAP_DEFAULT_TX_OCTETS,GAP_DEFAULT_TX_TIME );
		return (events ^ DLE_UPDATE_EVT);		
	}
	#endif
	#if( defined(GAP_CONFIG_ENABLE_CHANGE_PHY) && GAP_CONFIG_ENABLE_CHANGE_PHY)
	if ( events & PHY_UPDATE_EVT )
	{
		if( gapRole_ConnectionHandle != INVALID_CONNHANDLE )
		{
			HCI_LE_SetDefaultPhyMode( gapRole_ConnectionHandle,0,GAP_DEFAULT_PRE_PHY, GAP_DEFAULT_PRE_PHY);
			HCI_LE_SetPhyMode(gapRole_ConnectionHandle, 0, GAP_DEFAULT_PRE_PHY, GAP_DEFAULT_PRE_PHY, 0);
		}
		return (events ^ PHY_UPDATE_EVT);		
	}
	#endif

	#if(defined(GAP_DEFAULT_ENABLE_PARAM_UPDATE) && GAP_DEFAULT_ENABLE_PARAM_UPDATE )
	if ( events & PARAM_UPDATE_EVT )
	{
		if( gapRole_ConnectionHandle != INVALID_CONNHANDLE )
		{
			l2capParamUpdateReq_t updateReq;
			uint16 timeout = GAP_GetParamValue( TGAP_CONN_PARAM_TIMEOUT );
			updateReq.intervalMin = GAP_DEFAULT_DESIRED_MIN_CONN_INTERVAL;
			updateReq.intervalMax = GAP_DEFAULT_DESIRED_MAX_CONN_INTERVAL;
			updateReq.slaveLatency = GAP_DEFAULT_DESIRED_SLAVE_LATENCY;
			updateReq.timeoutMultiplier = GAP_DEFAULT_DESIRED_CONN_TIMEOUT;
			L2CAP_ConnParamUpdateReq( gapRole_ConnectionHandle, &updateReq, gapRole_TaskID );
		}
		return (events ^ PARAM_UPDATE_EVT);
	}
	#endif
#endif
	// Discard unknown events
	return 0;
}

/*********************************************************************
 * @fn      gapRole_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void gapRole_ProcessOSALMsg( osal_event_hdr_t *pMsg )
{
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
#if(defined(GAP_DEFAULT_MSG_BYPASS_ENABLE) && GAP_DEFAULT_MSG_BYPASS_ENABLE)
	// bypass
#else
	switch ( pMsg->event )
	{
		case GAP_MSG_EVENT:
			gapRole_ProcessGAPMsg( (gapEventHdr_t *)pMsg );
		break;
		case HCI_GAP_EVENT_EVENT:
		{
			switch( pMsg->status )
			{
				case HCI_COMMAND_COMPLETE_EVENT_CODE :
				{
					#ifdef _PHY_DEBUG 
						hciEvt_CmdComplete_t *pPkt = (hciEvt_CmdComplete_t *)pMsg;
						LOG("	HCI_COMMAND_COMPLETE_EVENT_CODE\n");
						if ( pPkt->cmdOpcode == HCI_READ_RSSI )
						{
							LOG("	Read RSSI %d\n",(int8)pPkt->pReturnParam[3] );
						}
						else if(pPkt->cmdOpcode == HCI_LE_SET_DATA_LENGTH )
						{
							LOG("	Set DLE Change status %d\n",pPkt->pReturnParam[0]);
						}
						else
							LOG("	Unknown cmdOpcode %d\n",pPkt->cmdOpcode );
					#endif
				}
				break;
				case HCI_COMMAND_STATUS_EVENT_CODE:
				{
					#ifdef _PHY_DEBUG 
							hciEvt_CommandStatus_t *pPkt = (hciEvt_CommandStatus_t *)pMsg;
							LOG("	HCI_COMMAND_STATUS_EVENT_CODE\n");
							if ( pPkt->cmdOpcode == HCI_LE_SET_PHY )
							{
								LOG("	Set PHY status 0x%02X\n",pPkt->cmdStatus);
							}
							else
								LOG("	Unknown cmdOpcode %d\n",pPkt->cmdOpcode );
					#endif
				}
				break;
				case HCI_LE_EVENT_CODE:
				{
					hciEvt_BLEEvent_Hdr_t *pPkt = (hciEvt_BLEEvent_Hdr_t *)pMsg;
		            if ( pPkt->BLEEventCode == HCI_BLE_DATA_LENGTH_CHANGE_EVENT )
		            {
		            	#ifdef _PHY_DEBUG
							hciEvt_BLEDataLenChange_t * pkt = (hciEvt_BLEDataLenChange_t *)pMsg;
							LOG("	Data Length Change Event\n");
							LOG("	Max Tx Octets : %d Byte\n",pkt->MaxTxOctets);
							LOG("	Max Rx Octets : %d Byte\n",pkt->MaxRxOctets);
							LOG("	Max Tx Time   : %d us\n",pkt->MaxTxTime);
							LOG("	Max Rx Time   : %d us\n",pkt->MaxRxTime);
						#endif
		            }
		            else if(pPkt->BLEEventCode == HCI_BLE_PHY_UPDATE_COMPLETE_EVENT )
		            {
		            	#ifdef _PHY_DEBUG
							hciEvt_BLEPhyUpdateComplete_t * pkt = (hciEvt_BLEPhyUpdateComplete_t *)pMsg;
							LOG("	PHY Change Event\n");
							LOG("	Tx PHY: %d\n",pkt->txPhy);
							LOG("	Tx PHY: %d\n",pkt->rxPhy);
						#endif
		            }
				}
				break;
			}
		}
		break;
		case L2CAP_SIGNAL_EVENT:
      	{
	        l2capSignalEvent_t *pPkt = (l2capSignalEvent_t *)pMsg;

	        // Process the Parameter Update Response
	        if ( pPkt->opcode == L2CAP_PARAM_UPDATE_RSP )
	        {
	        	#ifdef _PHY_DEBUG
					l2capParamUpdateRsp_t *pRsp = (l2capParamUpdateRsp_t *)&(pPkt->cmd.updateRsp);
					LOG("	L2CAP Parameter Update RSP\n");
					LOG("	Resault: %d\n",pRsp->result);
					if( pRsp->result == L2CAP_CONN_PARAMS_ACCEPTED )
					{
						LOG("	L2CAP Parameter Update Success \n");
					}
					else
					{
						LOG("	L2CAP Parameter Update failure... \n");

						// user Logic can be add
						// eg. re-L2CAP Parameter request
						// eg. terminate connection
					}
				#endif
	        }
		}
		break;
		default:
		#ifdef _PHY_DEBUG
			LOG( "	Unknown Msg Event 0x%X\n",pMsg->event);
		#endif
	break;
  }
#endif
}

/*********************************************************************
 * @fn      gapRole_ProcessGAPMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void gapRole_ProcessGAPMsg( gapEventHdr_t *pMsg )
{
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
#if(defined(GAP_DEFAULT_MSG_BYPASS_ENABLE) && GAP_DEFAULT_MSG_BYPASS_ENABLE)

#else
	switch ( pMsg->opcode )
	{
		case GAP_DEVICE_INIT_DONE_EVENT:
		{
			gapDeviceInitDoneEvent_t *pPkt = (gapDeviceInitDoneEvent_t *)pMsg;
//			#ifdef _PHY_DEBUG 
				LOG( "Device Address(LSB to MSB): " );
				LOG_DUMP_BYTE(pPkt->devAddr,B_ADDR_LEN);
//			#endif
			if ( pPkt->hdr.status == SUCCESS )
			{
				
				#if( defined(GAP_CONFIG_STATIC_ADDR) && GAP_CONFIG_STATIC_ADDR)
					GAP_ConfigDeviceAddr(ADDRTYPE_STATIC,BD_STATIC_ADDR);
				#endif
				#if ( HOST_CONFIG & PERIPHERAL_CFG )
					{
						#if(defined(GAP_CONFIG_HID_ENABLE) && GAP_CONFIG_HID_ENABLE)
							uint8 advertData[] ={0x02,GAP_ADTYPE_FLAGS,DEFAULT_DISCOVERABLE_MODE,
													// appearance
													0x03,	// length of this data
													GAP_ADTYPE_APPEARANCE,
													LO_UINT16(GAP_APPEARE_HID_GAMEPAD),
													HI_UINT16(GAP_APPEARE_HID_GAMEPAD),
							
													// service UUIDs
													0x03,	// length of this data
													GAP_ADTYPE_16BIT_COMPLETE,
													LO_UINT16(HID_SERV_UUID),
													HI_UINT16(HID_SERV_UUID)};							
						#else
							uint8 advertData[] ={
										0x02,   // length of this data
										GAP_ADTYPE_FLAGS,
										DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

										0x0B, 
										GAP_ADTYPE_LOCAL_NAME_COMPLETE, 
										'E', 'L', 'K', '-', 'B', 'L', 'E', 'D', 'O', 'M',
								};
						#endif
						GAP_UpdateAdvertisingData( GAP_DEFAULT_MSG_BYPASS_ID,TRUE, sizeof(advertData), advertData );
					}
				#endif
			}
		}
		break;
		#if ( HOST_CONFIG & PERIPHERAL_CFG )
			case GAP_ADV_DATA_UPDATE_DONE_EVENT:
			{
				gapAdvDataUpdateEvent_t *pPkt = (gapAdvDataUpdateEvent_t *)pMsg;
				uint8 status = 0xFF;
				#ifdef _PHY_DEBUG 
					LOG("%s,pMsg->opcode:0x%04X GAP advertising data update done,status 0x%X\n",__func__,pMsg->opcode,pPkt->hdr.status );
				#endif
				if ( pPkt->hdr.status == SUCCESS )
				{
					if ( pPkt->adType )
					{
						#if(defined(GAP_CONFIG_HID_ENABLE) && GAP_CONFIG_HID_ENABLE)
						uint8 scanRspData[] ={	0x03,   // length of this data
											    GAP_ADTYPE_APPEARANCE,
											    LO_UINT16(GAP_APPEARE_HID_GAMEPAD),
											    HI_UINT16(GAP_APPEARE_HID_GAMEPAD),

											    // service UUIDs
											    0x03,   // length of this data
											    GAP_ADTYPE_16BIT_COMPLETE,
											    LO_UINT16(HID_SERV_UUID),
											    HI_UINT16(HID_SERV_UUID),
											    0x09,   // length of this data
												GAP_ADTYPE_LOCAL_NAME_COMPLETE,
												'J','A','C','K','-','H','I','D' };
						#else
						uint8 scanRspData[] ={
												0x11, 
												GAP_ADTYPE_MANUFACTURER_SPECIFIC, 
												0xff, 0xff, 
												0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
												0x66, 0x02,	//	IC ID:	H66 OTP
												0xFD,		//	RGBLight
												0x03, 0x01,	//	production ID
												0x01, 0x00,	//	software version
												0x00,
											};
							uint8 publicAddr[B_ADDR_LEN];
							#if( defined(GAP_CONFIG_STATIC_ADDR) && GAP_CONFIG_STATIC_ADDR)
								osal_memcpy(publicAddr],BD_STATIC_ADDR,B_ADDR_LEN );
							#else
							{
								LL_ReadBDADDR(publicAddr);
								osal_memcpy(publicAddr,publicAddr,B_ADDR_LEN );
							}
							#endif
							{
								uint8	index;
								for(index = 0;index < B_ADDR_LEN;index++)
								{
									scanRspData[4 + index]	=	publicAddr[5 - index];
								}
							}
							// osal_memcpy(&scanRspData[7],AddrStr,(B_ADDR_LEN<<1) );
						#endif
						status = GAP_UpdateAdvertisingData( GAP_DEFAULT_MSG_BYPASS_ID,FALSE, sizeof(scanRspData), scanRspData);
						if( SUCCESS != status )
						{
							#ifdef _PHY_DEBUG 
								LOG("GAP update scan response data failure status 0x%X\n",status);
							#endif
						}
						else
						{
							// make discoverable
							#ifdef _PHY_DEBUG 
								LOG("GAP update scan response data success... \n");
							#endif
							uint16 advInt = 160;							
							VOID GAP_SetParamValue( TGAP_LIM_DISC_ADV_INT_MIN, advInt );
							VOID GAP_SetParamValue( TGAP_LIM_DISC_ADV_INT_MAX, advInt );
							VOID GAP_SetParamValue( TGAP_GEN_DISC_ADV_INT_MIN, advInt );
							VOID GAP_SetParamValue( TGAP_GEN_DISC_ADV_INT_MAX, advInt );
 
							gapAdvertisingParams_t advParams={
												.eventType = GAP_DEFAULT_ADV_TYPE,
												.initiatorAddrType = GAP_DEFAULT_ADV_ADDR_TYPE ,
												.initiatorAddr = GAP_DEFAULT_ADV_INIT_ADDR,
												.channelMap = GAP_DEFAULT_ADV_CHNMAP,
												.filterPolicy = GAP_DEFAULT_ADV_POLICY,
												};
							status = GAP_MakeDiscoverable( GAP_DEFAULT_MSG_BYPASS_ID, &advParams );
							#ifdef _PHY_DEBUG 
							if(  status != SUCCESS )
								LOG("GAP make advertising discoverable error , status 0x%X\n",status);
							else
								LOG("GAP Make advertising discoverable...\n");
							#endif
						}
					}
				}
			}
			break;			
			case GAP_MAKE_DISCOVERABLE_DONE_EVENT:
			{
//				#ifdef _PHY_DEBUG 
					gapMakeDiscoverableRspEvent_t *pPkt = (gapMakeDiscoverableRspEvent_t *)pMsg;
					LOG("%s,pMsg->opcode:0x%04X GAP Make advertising discoverable done,status 0x%X\n",__func__,pMsg->opcode,pPkt->hdr.status );
					if( pPkt->hdr.status == SUCCESS )
					{
						LOG("GAP Advertising started...\n");
					}
					else
						LOG("GAP Advertising start failure\n");
//				#endif
			}
			break;
			case GAP_END_DISCOVERABLE_DONE_EVENT:
			{
//				#ifdef _PHY_DEBUG 
					gapMakeDiscoverableRspEvent_t *pPkt = (gapMakeDiscoverableRspEvent_t *)pMsg;
					LOG("%s,pMsg->opcode:0x%04X GAP End advertising discoverable done,status 0x%X\n",__func__,pMsg->opcode,pPkt->hdr.status );
				if( pPkt->hdr.status == SUCCESS )
					LOG("GAP Advertising stoped...\n");
				else
					LOG("GAP Advertising stop failure\n");
//				#endif
			}
			break;
			case GAP_LINK_ESTABLISHED_EVENT:
			{
				gapEstLinkReqEvent_t *pPkt = (gapEstLinkReqEvent_t *)pMsg;

				gapRole_ConnectionHandle = pPkt->connectionHandle;
//				#ifdef _PHY_DEBUG 
					LOG("%s,pMsg->opcode:0x%04X GAP Link Established,status 0x%X\n",__func__,pMsg->opcode,pPkt->hdr.status );
					LOG("	connection handle 		%d\n",pPkt->connectionHandle);
					LOG("	connection interval 	%d\n",pPkt->connInterval);
					LOG("	connection latency 	%d\n",pPkt->connLatency);
					LOG("	connection timeout 	%d\n",pPkt->connTimeout);
					LOG("	connection devAddrType	%d,device Address:0x",pPkt->devAddrType);
					for(uint8 i=0;i<B_ADDR_LEN;i++)
						LOG("%02X",pPkt->devAddr[i]);
					LOG("\n");					
//				#endif
				#if(defined(GAP_DEFAULT_ENABLE_PARAM_UPDATE) && GAP_DEFAULT_ENABLE_PARAM_UPDATE )
					osal_start_timerEx(GAP_DEFAULT_MSG_BYPASS_ID, PARAM_UPDATE_EVT,GAP_DEFAULT_PARAM_UPDATE_PAUSE*1000 );
				#endif
				#if (defined( GAP_DEFAULT_ENABLE_SMP ) && GAP_DEFAULT_ENABLE_SMP )
					GAPBondMgr_LinkEst( pPkt->connectionHandle, GAP_PROFILE_PERIPHERAL );
				#endif
				#if (defined(GAP_DEFAULT_ENABLE_SEC_ADV) && GAP_DEFAULT_ENABLE_SEC_ADV )
					if( SUCCESS == pPkt->hdr.status )
					{
						gapAdvertisingParams_t advParams={
												.eventType = GAP_DEFAULT_SEC_ADV_TYPE,
												.initiatorAddrType = GAP_DEFAULT_ADV_ADDR_TYPE ,
												.initiatorAddr = GAP_DEFAULT_ADV_INIT_ADDR,
												.channelMap = GAP_DEFAULT_ADV_CHNMAP,
												.filterPolicy = GAP_DEFAULT_ADV_POLICY,
												};
						uint8 status = GAP_MakeDiscoverable( GAP_DEFAULT_MSG_BYPASS_ID, &advParams );
						if (  status != SUCCESS )
						{
							#ifdef _PHY_DEBUG 
								LOG("GAP make secondary advertising discoverable error , status 0x%X\n",status);
							#endif
						}
						else
						{
							#ifdef _PHY_DEBUG 
								LOG("GAP Make secondary advertising discoverable...\n");
							#endif
						}
					}
				#endif
				#if( defined(GAP_CONFIG_ENABLE_DLE) && GAP_CONFIG_ENABLE_DLE)
					osal_start_timerEx(GAP_DEFAULT_MSG_BYPASS_ID, DLE_UPDATE_EVT,GAP_DEFAULT_START_DLE );
				#endif
				#if( defined(GAP_CONFIG_ENABLE_CHANGE_PHY) && GAP_CONFIG_ENABLE_CHANGE_PHY)
					osal_start_timerEx(GAP_DEFAULT_MSG_BYPASS_ID, PHY_UPDATE_EVT,GAP_DEFAULT_START_CHANGE_PHY);
				#endif
				#if(defined(GAP_CONFIG_PERIODIC_READ_RSSI) && GAP_CONFIG_PERIODIC_READ_RSSI)
					osal_start_reload_timer(GAP_DEFAULT_MSG_BYPASS_ID, RSSI_READ_EVT , GAP_DEFAULT_READ_RSSI_INTV );
				#endif
			}
			break;
			case GAP_LINK_TERMINATED_EVENT:
			{
				gapRole_ConnectionHandle = INVALID_CONNHANDLE;
				#if( defined(GAP_CONFIG_ENABLE_DLE) && GAP_CONFIG_ENABLE_DLE)
					osal_stop_timerEx(GAP_DEFAULT_MSG_BYPASS_ID, DLE_UPDATE_EVT );
				#endif
				#if( defined(GAP_CONFIG_ENABLE_CHANGE_PHY) && GAP_CONFIG_ENABLE_CHANGE_PHY)
					osal_stop_timerEx(GAP_DEFAULT_MSG_BYPASS_ID, PHY_UPDATE_EVT );
				#endif
				#if(defined(GAP_CONFIG_PERIODIC_READ_RSSI) && GAP_CONFIG_PERIODIC_READ_RSSI)
					osal_stop_timerEx(GAP_DEFAULT_MSG_BYPASS_ID, RSSI_READ_EVT);
				#endif
				#if(defined(GAP_DEFAULT_ENABLE_PARAM_UPDATE) && GAP_DEFAULT_ENABLE_PARAM_UPDATE )
					osal_stop_timerEx(GAP_DEFAULT_MSG_BYPASS_ID, PARAM_UPDATE_EVT);
				#endif
//				#ifdef _PHY_DEBUG 
					gapTerminateLinkEvent_t *pPkt = (gapTerminateLinkEvent_t *)pMsg;
					LOG("%s,pMsg->opcode:0x%04X GAP Link Terminated,status 0x%X\n",__func__,pMsg->opcode,pPkt->hdr.status );
					if( pPkt->hdr.status == SUCCESS )
					{
						LOG("	connection handle	%d\n",pPkt->connectionHandle);
						LOG("	reason				%d\n",pPkt->reason);
					}
//				#endif
				gapAdvertisingParams_t advParams={
												.eventType = GAP_DEFAULT_ADV_TYPE,
												.initiatorAddrType = GAP_DEFAULT_ADV_ADDR_TYPE ,
												.initiatorAddr = GAP_DEFAULT_ADV_INIT_ADDR,
												.channelMap = GAP_DEFAULT_ADV_CHNMAP,
												.filterPolicy = GAP_DEFAULT_ADV_POLICY,
												};
				GAP_MakeDiscoverable( GAP_DEFAULT_MSG_BYPASS_ID, &advParams );
			}
			break;
			case GAP_LINK_PARAM_UPDATE_EVENT:
			{
				#ifdef _PHY_DEBUG 
					gapLinkUpdateEvent_t *pPkt = (gapLinkUpdateEvent_t *)pMsg;
					LOG("%s,pMsg->opcode:0x%04X GAP Link Parameter update ,status 0x%X\n",__func__,pMsg->opcode,pPkt->hdr.status );
					if( pPkt->hdr.status == SUCCESS )
					{
						LOG("	connection handle		%d\n",pPkt->connectionHandle);
						LOG("	connection connIntv 	%d\n",pPkt->connInterval);
						LOG("	connection latency		%d\n",pPkt->connLatency);
						LOG("	connection timeout		%d\n",pPkt->connTimeout);
					}
				#endif
			}
			break;
		#endif
		#if ( HOST_CONFIG & OBSERVER_CFG  )
		case GAP_DEVICE_INFO_EVENT:
		{
			#ifdef _PHY_DEBUG 
				gapDeviceInfoEvent_t* pPkt = (gapDeviceInfoEvent_t*)pMsg;
				LOG("%s,pMsg->opcode:0x%04X GAP Device Info ,status 0x%X\n",__func__,pMsg->opcode,pPkt->hdr.status );
				LOG("	Device Address : ");
				for(uint8 i=0;i<B_ADDR_LEN;i++)
					LOG("%02X",pPkt->addr[i]);
				LOG("\n");
				LOG("	eventType %d,addrType %d,rssi %d,dataLen %d\n",pPkt->eventType,pPkt->addrType,pPkt->rssi,pPkt->dataLen);
				LOG("	Data:");
				for(uint8 i = 0 ; i < pPkt->dataLen; i++)
					LOG("%02X",pPkt->pEvtData[i]);
				LOG("\n");
			#endif
		}
		break;
		case GAP_DEVICE_DISCOVERY_EVENT:
		{
			#ifdef _PHY_DEBUG 
				LOG("%s,pMsg->opcode:0x%04X GAP Device Discovery Dnne...\n",__func__,pMsg->opcode);
			#endif
		}
		break;
		#endif
		default:
			#ifdef _PHY_DEBUG 
				LOG("%s,pMsg->opcode:0x%04X GAP Device Info \n",__func__,pMsg->opcode);
			#endif
		break;
	}
#endif
}

/*********************************************************************
 * @fn      gapRole_SetupGAP
 *
 * @brief   Call the GAP Device Initialization function using the
 *          Profile Parameters.
 *
 * @param   none
 *
 * @return  none
 */
static void gapRole_SetupGAP( void )
{
	uint8 gapRole_IRK[16];
	uint8 gapRole_SRK[16];
	uint32 gapRole_signCounter;

	GAP_DeviceInit( GAP_DEFAULT_MSG_BYPASS_ID,
		GAP_PROFILE_ROLE, DEFAULT_MAX_SCAN_RES,
		gapRole_IRK, gapRole_SRK,
		&gapRole_signCounter );
}

