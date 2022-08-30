/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

#if( defined( SERV_BATT_SERV ) && SERV_BATT_SERV )

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "types.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gatt_profile_uuid.h"
#include "gattservapp.h"
#include "peripheral.h"
#include "hiddev.h"
#include "battservice.h"
#include "rom_sym_def.h"
#include "log.h"
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// ADC voltage levels
#define BATT_ADC_LEVEL_3V           409
#define BATT_ADC_LEVEL_2V           273

#define BATT_LEVEL_VALUE_IDX        2 // Position of battery level in attribute array
#define BATT_LEVEL_VALUE_CCCD_IDX   3 // Position of battery level CCCD in attribute array

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Battery service
CONST uint8 battServUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BATT_SERV_UUID), HI_UINT16(BATT_SERV_UUID)
};

// Battery level characteristic
CONST uint8 battLevelUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BATT_LEVEL_UUID), HI_UINT16(BATT_LEVEL_UUID)
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

//// Application callback
//static battServiceCB_t battServiceCB;

// Critical battery level setting
//static uint8 battCriticalLevel;

// ADC channel to be used for reading

/*********************************************************************
 * Profile Attributes - variables
 */

// Battery Service attribute
static CONST gattAttrType_t battService = { ATT_BT_UUID_SIZE, battServUUID };

// Battery level characteristic
static uint8 battLevelProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
static uint8 battLevel = 99;

static uint16 battLevelClientCharCfg = GATT_CFG_NO_OPERATION;
// HID Report Reference characteristic descriptor, battery level
static uint8 hidReportRefBattLevel[HID_REPORT_REF_LEN] = { HID_RPT_ID_BATT_LEVEL_IN, HID_REPORT_TYPE_INPUT };

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t battAttrTbl[] =
{
  // Battery Service
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&battService                     /* pValue */
  },

    // Battery Level Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &battLevelProps
    },

      // Battery Level Value
      {
        { ATT_BT_UUID_SIZE, battLevelUUID },
        GATT_PERMIT_READ,
        0,
        &battLevel
      },

      // Battery Level Client Characteristic Configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *) &battLevelClientCharCfg
      },

      // HID Report Reference characteristic descriptor, batter level input
      {
        { ATT_BT_UUID_SIZE, reportRefUUID },
        GATT_PERMIT_READ,
        0,
        hidReportRefBattLevel
      },
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 battReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                             uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t battWriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                  uint8 *pValue, uint8 len, uint16 offset );
static void battNotifyCB( linkDBItem_t *pLinkItem );
static uint8 battMeasure( void );
static void battNotifyLevel( void );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Battery Service Callbacks
CONST gattServiceCBs_t battCBs =
{
  battReadAttrCB,  // Read callback function pointer
  battWriteAttrCB, // Write callback function pointer
  NULL             // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Batt_AddService
 *
 * @brief   Initializes the Battery Service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 */
bStatus_t Batt_AddService( void )
{
  uint8 status = SUCCESS;

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( battAttrTbl,
                                        GATT_NUM_ATTRS( battAttrTbl ),
                                        &battCBs );

  return ( status );
}


/*********************************************************************
 * @fn          Batt_MeasLevel
 *
 * @brief       Measure the battery level and update the battery
 *              level value in the service characteristics.  If
 *              the battery level-state characteristic is configured
 *              for notification and the battery level has changed
 *              since the last measurement, then a notification
 *              will be sent.
 *
 * @return      Success
 */
bStatus_t Batt_MeasLevel( void )
{
  	uint8 level;
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif

	level = battMeasure();

	// If level has gone down
	if (level < battLevel)
	{
		// Update level
		battLevel = level;

		// Send a notification
		battNotifyLevel();
	}

	return SUCCESS;
}



/*********************************************************************
 * @fn          battReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 battReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                             uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  	bStatus_t status = SUCCESS;
	#ifdef _PHY_DEBUG 
		  LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif

	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}

  	uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1] );
	#ifdef _PHY_DEBUG 
		  LOG("uuid 0x%X\n",uuid );
	#endif

	// Measure battery level if reading level
	if ( uuid == BATT_LEVEL_UUID )
	{
		uint8 level;

		level = battMeasure();

		// If level has gone down
		if (level < battLevel)
		{
		// Update level
		battLevel = level;
		}

		*pLen = 1;
		pValue[0] = battLevel;
	}
	else if ( uuid == GATT_REPORT_REF_UUID )
	{
//		*pLen = HID_RPT_ID_BATT_LEVEL_IN;//HID_REPORT_REF_LEN;
//		osal_memcpy( pValue, pAttr->pValue, /*HID_REPORT_REF_LEN*/HID_RPT_ID_BATT_LEVEL_IN );
		*pLen = 1;
		pValue[0] = battMeasure();

	}
	else
	{
		status = ATT_ERR_ATTR_NOT_FOUND;
	}

  return ( status );
}

/*********************************************************************
 * @fn      battWriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  Success or Failure
 */
static bStatus_t battWriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                  uint8 *pValue, uint8 len, uint16 offset )
{
	bStatus_t status = SUCCESS;

	#ifdef _PHY_DEBUG 
		  LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
	
	uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
	#ifdef _PHY_DEBUG 
		LOG("		uuid 0x%X,value:",uuid );
		for(uint8 i=0;i<len;i++)
			LOG("0x%02X,",pValue[i]);
		LOG("\n");
	#endif
	switch ( uuid )
	{
		case GATT_CLIENT_CHAR_CFG_UUID:
			battLevelClientCharCfg = BUILD_UINT16( pValue[0], pValue[1] );
		break;

		default:
			status = ATT_ERR_ATTR_NOT_FOUND;
		break;
	}

	return ( status );
}

/*********************************************************************
 * @fn          battNotifyCB
 *
 * @brief       Send a notification of the level state characteristic.
 *
 * @param       connHandle - linkDB item
 *
 * @return      None.
 */
static void battNotifyCB( linkDBItem_t *pLinkItem )
{
	#ifdef _PHY_DEBUG 
		  LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif
	if ( pLinkItem->stateFlags & LINK_CONNECTED )
	{
		if ( battLevelClientCharCfg & GATT_CLIENT_CFG_NOTIFY )
		{
			attHandleValueNoti_t noti;

			noti.handle = battAttrTbl[BATT_LEVEL_VALUE_IDX].handle;
			noti.len = 1;
			noti.value[0] = battLevel;

			GATT_Notification( pLinkItem->connectionHandle, &noti, FALSE );
		}
	}
}

/*********************************************************************
 * @fn      battMeasure
 *
 * @brief   Measure the battery level with the ADC and return
 *          it as a percentage 0-100%.
 *
 * @return  Battery level.
 */
static uint8 battMeasure( void )
{
	uint8 percent;
	percent = 95;

	return percent;
}

/*********************************************************************
 * @fn      battNotifyLevelState
 *
 * @brief   Send a notification of the battery level state
 *          characteristic if a connection is established.
 *
 * @return  None.
 */
static void battNotifyLevel( void )
{
  // Execute linkDB callback to send notification
  linkDB_PerformFunc( battNotifyCB );
}

#endif

/*********************************************************************
*********************************************************************/
