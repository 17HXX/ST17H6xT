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

extern void SimpleGATTProfile_Notify(uint16 connHandle,uint8 len, void* value );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEGATTPROFILE_H */
