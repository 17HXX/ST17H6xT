/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/

/**************************************************************************************************
    Filename:      
    Revised:
    Revision:

    Description:    This file contains the Simple GATT profile sample GATT service
                  profile for use with the BLE sample application.


**************************************************************************************************/

/*********************************************************************
    INCLUDES
*/
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"
#include "simpleGATTprofile.h"
#include "log.h"
#include "LC_Common.h"
#include "LC_UI_led_buzzer.h"
/*********************************************************************
    MACROS
*/
#define SIMPLEGATTPROFILE_NOTIFY_IDX		4
#define SIMPLEGATT_VALUE_SIZE				20

/*********************************************************************
    CONSTANTS
*/

/*********************************************************************
    TYPEDEFS
*/

/*********************************************************************
    GLOBAL VARIABLES
*/
CONST uint8 simpleGATTProfileServUUID[ATT_BT_UUID_SIZE] =  {0xF0,0xFF};
CONST uint8 simpleGATTProfilechar1UUID[ATT_BT_UUID_SIZE] = {0xF3,0xFF};
CONST uint8 simpleGATTProfilechar2UUID[ATT_BT_UUID_SIZE] = {0xF4,0xFF};


/*********************************************************************
    LOCAL VARIABLES
*/
/*********************************************************************
    Profile Attributes - variables
*/
// multi Profile Service attribute
static CONST gattAttrType_t simpleGATTProfileService = { ATT_BT_UUID_SIZE, simpleGATTProfileServUUID };

// simpleGATTProfile Characteristic 1 Properties
static uint8 simpleGATTProfileChar1Props = GATT_PROP_READ | GATT_PROP_WRITE_NO_RSP;
// Characteristic 1 Value
static uint8 simpleGATTProfileChar1[ SIMPLEGATT_VALUE_SIZE ];

// simpleGATTProfile Characteristic 2 Properties
static uint8 simpleGATTProfileChar2Props = GATT_PROP_NOTIFY;
// Characteristic 2 Value
static uint8 simpleGATTProfileChar2[ SIMPLEGATT_VALUE_SIZE];
// Characteristic 2 cccd
static uint16 simpleGATTProfileChar2Config = GATT_CFG_NO_OPERATION;


/*********************************************************************
    Profile Attributes - Table
*/
static gattAttribute_t simpleGATTProfileAttrTbl[] =
{
	/* type */											/* permissions */				/* handle */	/* pValue */
    // multi Profile Service
    {{ ATT_BT_UUID_SIZE,	primaryServiceUUID },		GATT_PERMIT_READ,						0,		(uint8*)&simpleGATTProfileService},

    // Characteristic 1 Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID },				GATT_PERMIT_READ,						0,		&simpleGATTProfileChar1Props},
    // Characteristic Value 1
    {{ ATT_BT_UUID_SIZE, simpleGATTProfilechar1UUID },	GATT_PERMIT_READ | GATT_PERMIT_WRITE,	0,		&simpleGATTProfileChar1[0]},

    // ----------------------------------------------------------------------
    // Characteristic 2 Declaration, NOTify
    {{ ATT_BT_UUID_SIZE, characterUUID },				GATT_PERMIT_READ,						0,		&simpleGATTProfileChar2Props},
    // Characteristic Value 2
    {{ ATT_BT_UUID_SIZE, simpleGATTProfilechar2UUID },	GATT_PERMIT_READ,						0,		(uint8*)&simpleGATTProfileChar2},
    // Characteristic 2 configuration
    {{ ATT_BT_UUID_SIZE, clientCharCfgUUID },			GATT_PERMIT_READ | GATT_PERMIT_WRITE,	0,		(uint8*)(&simpleGATTProfileChar2Config)},
};

/*********************************************************************
    LOCAL FUNCTIONS
*/
static	bStatus_t	simpleGATTProfile_ReadAttrCB(uint16 connHandle, gattAttribute_t* pAttr, uint8* pValue, uint16* pLen, uint16 offset, uint8 maxLen);
static	bStatus_t	simpleGATTProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t* pAttr, uint8* pValue, uint8 len, uint16 offset );


/*********************************************************************
    PROFILE CALLBACKS
*/
// multi Profile Service Callbacks
CONST gattServiceCBs_t simpleGATTProfileCBs =
{
    simpleGATTProfile_ReadAttrCB,	// Read callback function pointer
    simpleGATTProfile_WriteAttrCB, 	// Write callback function pointer
    NULL							// Authorization callback function pointer
};

/*********************************************************************
    PUBLIC FUNCTIONS
*/

/*********************************************************************
    @fn      multiProfile_AddService

    @brief   Initializes the multi Profile service by registering
            GATT attributes with the GATT server.

    @param   services - services to add. This is a bit map and can
                       contain more than one service.

    @return  Success or Failure
*/
bStatus_t simpleGATTProfile_AddService()
{
    bStatus_t status = SUCCESS;
    status = GATTServApp_RegisterService( simpleGATTProfileAttrTbl,
                                          GATT_NUM_ATTRS( simpleGATTProfileAttrTbl ),
                                          &simpleGATTProfileCBs );
    return ( status );
}
/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t	simpleGATTProfile_SetParamter(uint8 param, uint8 len, void* value)
{
	bStatus_t	ret	=	SUCCESS;

	switch(param)
	{
		case	SIMPLEGATTPROFILE_CHAR1:
			if(len < SIMPLEGATT_VALUE_SIZE)
			{
				osal_memcpy(simpleGATTProfileChar1, value, len);
			}
			else
			{
				ret	=	bleInvalidRange;
			}
		break;
		default:
			ret	=	INVALIDPARAMETER;
		break;
	}
	return	ret;

}

/*********************************************************************
 * @fn          simpleProfile_ReadAttrCB
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

static	bStatus_t	simpleGATTProfile_ReadAttrCB(uint16 connHandle, gattAttribute_t* pAttr,
												 uint8* pValue, uint16* pLen, uint16 offset, uint8 maxLen)
{
	bStatus_t	Status	=	SUCCESS;

	#ifdef _PHY_DEBUG 
	LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif

	// If attribute permissions require authorization to read, return error
	if(gattPermitAuthorRead(pAttr->permissions))
	{
		// Insufficient authorization
		return(ATT_ERR_INSUFFICIENT_AUTHOR);
	}

	// Make sure it's not a blob operation (no attributes in the profile are long)
	if(offset > 0)
	{
		return(ATT_ERR_ATTR_NOT_LONG);
	}

	if(pAttr->type.len == ATT_BT_UUID_SIZE)
	{
		// 16-bit UUID
		uint16	uuid	=	BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);
		switch(uuid)
		{
			// No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
			// gattserverapp handles those reads
			case	SIMPLEGATTPROFILE_CHAR1_UUID:
				*pLen	=	LC_App_Set_Param.app_write_len;
				osal_memcpy(pValue, pAttr->pValue, *pLen);
				LOG("read char1\n");
			break;
			// case	SIMPLEGATTPROFILE_CHAR2_UUID:
			// 	*pLen = LC_App_Set_Param.app_notify_len;
			// 	VOID osal_memcpy( pValue, pAttr->pValue, *pLen );
			// 	LOG("Read_UUID2:\n");
			// break;
			default:
				// Should never get here! (characteristics 3 and 4 do not have read permissions)
			break;
		}
	}
	else
	{
		*pLen	=	0;
		Status	=	ATT_ERR_INVALID_HANDLE;
	}
	return(Status);
}

/*********************************************************************
    @fn      simpleGATTProfile_WriteAttrCB

    @brief   Validate attribute data prior to a write operation

    @param   connHandle - connection message was received on
    @param   pAttr - pointer to attribute
    @param   pValue - pointer to data to be written
    @param   len - length of data
    @param   offset - offset of the first octet to be written

    @return  Success or Failure
*/
static bStatus_t simpleGATTProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t* pAttr,
                                           uint8* pValue, uint8 len, uint16 offset )
{
	bStatus_t status = SUCCESS;

	#ifdef _PHY_DEBUG 
	LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
	#endif

    // If attribute permissions require authorization to write, return error
    if ( gattPermitAuthorWrite( pAttr->permissions ) )
    {
        // Insufficient authorization
        return ( ATT_ERR_INSUFFICIENT_AUTHOR );
    }
	

    if ( pAttr->type.len == ATT_BT_UUID_SIZE )
    {
    	#ifdef _PHY_DEBUG 
			LOG("	connection handle %d\n",connHandle);
			LOG("	uuid len %d,value:",pAttr->type.len);
			for(uint8 i=0;i < pAttr->type.len;i++)
				LOG("0x%02X,",pAttr->type.uuid[i]);
			LOG("\n");
			LOG("	write data len %d, value : ",len);
			for(uint8 i=0;i < len;i++)
				LOG("0x%02X,",pValue[i]);
			LOG("\n");
		#endif
		// 16-bit UUID
		uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
		switch ( uuid )
		{
			case SIMPLEGATTPROFILE_CHAR1_UUID:
			{
				osal_memcpy(LC_App_Set_Param.app_write_data, pValue, len);
				LC_App_Set_Param.app_write_len	=	len;
				osal_set_event(LC_Ui_Led_Buzzer_TaskID, DEAL_APP_DATA_EVT);
			}
			break;

			case GATT_CLIENT_CHAR_CFG_UUID:
			{
				simpleGATTProfileChar2Config = BUILD_UINT16( pValue[0], pValue[1]);
			}
			break;
		}
		// SimpleGATTProfile_Notify(connHandle,len,(uint8* )pValue);
    }
    return status;
}

void SimpleGATTProfile_Notify(uint16 connHandle,uint8 len, void* value )
{
	#ifdef _PHY_DEBUG 
		LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
		LOG("	Check notify enable %d\n",simpleGATTProfileChar2Config);
		LOG("	Check notify data length %d\n",len);
		LOG("	Check notify data:");
		for(uint8 i=0;i<len;i++)
			LOG("0x%02X,",((uint8 *)value)[i]);
		LOG("\n");
	#endif
	if ( simpleGATTProfileChar2Config & GATT_CLIENT_CFG_NOTIFY )
    {
		attHandleValueNoti_t Notif;
        Notif.handle = simpleGATTProfileAttrTbl[SIMPLEGATTPROFILE_NOTIFY_IDX].handle;
        Notif.len = len;
		osal_memcpy(Notif.value, (uint8 *)value, len);
        GATT_Notification( connHandle, &Notif, FALSE );
		
	}
}

