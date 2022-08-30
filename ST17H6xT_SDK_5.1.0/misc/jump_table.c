/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/


/**************************************************************************************************
  Filename:       jump_table.c
  Revised:        
  Revision:       

  Description:    Jump table that holds function pointers and veriables used in ROM code.
                  

**************************************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#include "rom_sym_def.h"
#include "jump_function.h"
#include "global_config.h"
#include "OSAL_Tasks.h"
#include "rf_phy_driver.h"
#include "pwrmgr.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "log.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */
// jump table, this table save the function entry which will be called by ROM code
// item 1 - 4 for OSAL task entry
// item 224 - 255 for ISR(Interrupt Service Routine) entry
// others are reserved by ROM code
volatile  uint16_t jump_table_base[128] __attribute__((section("jump_table_mem_area")));


/*******************************************************************************
 * Prototypes
 */


/*******************************************************************************
 * LOCAL VARIABLES
 */


/*********************************************************************
 * EXTERNAL VARIABLES
 */
volatile uint32 global_config[SOFT_PARAMETER_NUM] __attribute__((section("global_config_area")));



