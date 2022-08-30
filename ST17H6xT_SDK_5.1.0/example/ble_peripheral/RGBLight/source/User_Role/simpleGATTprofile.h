/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/

/**************************************************************************************************
    Filename:      
    Revised:
    Revision:

    Description:    This file contains the Simple GATT profile definitions and
                  prototypes.

 **************************************************************************************************/

#ifndef SIMPLE_GATT_PROFILE_H
#define SIMPLE_GATT_PROFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
    INCLUDES
*/

/*********************************************************************
    CONSTANTS
*/

/*********************************************************************
    TYPEDEFS
*/

/*********************************************************************
    MACROS
*/
//	Profile Parameters
#define		SIMPLEGATTPROFILE_CHAR1			0
#define		SIMPLEGATTPROFILE_CHAR2			1

//	simpleGATTProfile_UUIDS
#define		SIMPLEGATTPROFILE_SER_UUID		0xFFF0

#define		SIMPLEGATTPROFILE_CHAR1_UUID	0xFFF3
#define		SIMPLEGATTPROFILE_CHAR2_UUID	0xFFF4
/*********************************************************************
    Profile Callbacks
*/


/*********************************************************************
    API FUNCTIONS
*/


/*
    SimpleProfile_AddService- Initializes the Simple GATT Profile service by registering
            GATT attributes with the GATT server.

    @param   services - services to add. This is a bit map and can
                       contain more than one service.
*/

extern bStatus_t simpleGATTProfile_AddService();
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
bStatus_t	simpleGATTProfile_SetParamter(uint8 param, uint8 len, void* value);
extern void SimpleGATTProfile_Notify(uint16 connHandle,uint8 len, void* value );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEGATTPROFILE_H */
