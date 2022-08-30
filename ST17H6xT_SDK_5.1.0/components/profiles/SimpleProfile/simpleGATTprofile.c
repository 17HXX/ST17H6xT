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
CONST uint8 simpleGATTProfilechar1UUID[ATT_BT_UUID_SIZE] = {0xF1,0xFF};
CONST uint8 simpleGATTProfilechar2UUID[ATT_BT_UUID_SIZE] = {0xF2,0xFF};


/*********************************************************************
    LOCAL VARIABLES
*/
/*********************************************************************
    Profile Attributes - variables
*/
// multi Profile Service attribute
static CONST gattAttrType_t simpleGATTProfileService = { ATT_BT_UUID_SIZE, simpleGATTProfileServUUID };

// simpleGATTProfile Characteristic 1 Properties
static uint8 simpleGATTProfileChar1Props = GATT_PROP_WRITE;
// Characteristic 1 Value
static uint8 simpleGATTProfileChar1[ SIMPLEGATT_VALUE_SIZE ];

// simpleGATTProfile Characteristic 2 Properties
static uint8 simpleGATTProfileChar2Props = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic 2 Value
static uint8 simpleGATTProfileChar2[ SIMPLEGATT_VALUE_SIZE];
// Characteristic 2 cccd
static uint16 simpleGATTProfileChar2Config = GATT_CFG_NO_OPERATION;


/*********************************************************************
    Profile Attributes - Table
*/
static gattAttribute_t simpleGATTProfileAttrTbl[] =
{
    // multi Profile Service
    {
        { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
        GATT_PERMIT_READ,                         /* permissions */
        0,                                        /* handle */
        (uint8*)&simpleGATTProfileService            /* pValue */
    },

    // Characteristic 1 Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &simpleGATTProfileChar1Props
    },

    // Characteristic Value 1
    {
        { ATT_BT_UUID_SIZE, simpleGATTProfilechar1UUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        &simpleGATTProfileChar1[0]
    },

    // ----------------------------------------------------------------------
    // Characteristic 2 Declaration, NOTify
    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &simpleGATTProfileChar2Props
    },

    // Characteristic Value 2
    {
        { ATT_BT_UUID_SIZE, simpleGATTProfilechar2UUID },
        GATT_PERMIT_READ,
        0,
        (uint8*)&simpleGATTProfileChar2
    },

    // Characteristic 2 configuration
    {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8*)(&simpleGATTProfileChar2Config)
    },
};

/*********************************************************************
    LOCAL FUNCTIONS
*/
static bStatus_t simpleGATTProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t* pAttr,
                                           uint8* pValue, uint8 len, uint16 offset );


/*********************************************************************
    PROFILE CALLBACKS
*/
// multi Profile Service Callbacks
CONST gattServiceCBs_t simpleGATTProfileCBs =
{
    NULL,  						// Read callback function pointer
    simpleGATTProfile_WriteAttrCB, 	// Write callback function pointer
    NULL                       	// Authorization callback function pointer
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
			case GATT_CLIENT_CHAR_CFG_UUID:
			{
				simpleGATTProfileChar2Config = BUILD_UINT16( pValue[0], pValue[1]);
			}
			break;
		}
		SimpleGATTProfile_Notify(connHandle,len,(uint8* )pValue);
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

