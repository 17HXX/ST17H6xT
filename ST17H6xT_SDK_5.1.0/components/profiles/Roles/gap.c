/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/*************************************************************************************************
  Filename:       gap.c
  Revised:         
  Revision:        

  Description:    This file contains the GAP Configuration API.


 
**************************************************************************************************/

#include "bcomdef.h"
#include "gap.h"
#include "sm.h"
//#include "log.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
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

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * Called to setup the device.  Call just once.
 *
 * Public function defined in gap.h.
 */
bStatus_t GAP_DeviceInit(  uint8 taskID,
                           uint8 profileRole,
                           uint8 maxScanResponses,
                           uint8 *pIRK,
                           uint8 *pSRK,
                           uint32 *pSignCounter )
{
	// Setup the device configuration parameters
	bStatus_t stat = GAP_ParamsInit( taskID, profileRole );
	if ( stat == SUCCESS )
	{

		#if ( HOST_CONFIG & OBSERVER_CFG  )
		{
			// Initialize GAP Central Device Manager
			GAP_CentDevMgrInit( maxScanResponses );
		}
		#endif

		#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
		{
			// Initialize GAP Peripheral Device Manager
			GAP_PeriDevMgrInit();

			#if ( HOST_CONFIG & PERIPHERAL_CFG )
			{
				// Initialize SM Responder
				SM_ResponderInit();
			}
			#endif
		}
		#endif
	}

	return ( stat );
}

/*********************************************************************
*********************************************************************/
