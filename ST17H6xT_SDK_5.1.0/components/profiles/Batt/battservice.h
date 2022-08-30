/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/


#ifndef BATTSERVICE_H
#define BATTSERVICE_H

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

// Battery Service Get/Set Parameters
#define BATT_PARAM_LEVEL                0
#define BATT_PARAM_CRITICAL_LEVEL       1
#define BATT_PARAM_SERVICE_HANDLE       2
#define BATT_PARAM_BATT_LEVEL_IN_REPORT 3

// HID Report IDs for the service
#define HID_RPT_ID_BATT_LEVEL_IN        4  // Battery Level input report ID


/*********************************************************************
 * TYPEDEFS
 */

// Battery Service callback function
typedef void (*battServiceCB_t)(uint8 event);

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */


/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      Batt_AddService
 *
 * @brief   Initializes the Battery service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 */
extern bStatus_t Batt_AddService( void );

/*********************************************************************
 * @fn      Batt_Register
 *
 * @brief   Register a callback function with the Battery Service.
 *
 * @param   pfnServiceCB - Callback function.
 *
 * @return  None.
 */
extern void Batt_Register( battServiceCB_t pfnServiceCB );


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
 * @return      Success or Failure
 */
extern bStatus_t Batt_MeasLevel( void );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BATTSERVICE_H */
