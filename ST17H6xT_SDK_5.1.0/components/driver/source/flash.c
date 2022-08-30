/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/
#include "rom_sym_def.h"
#include "spif.h"
#include "otp.h"
#include "error.h"
#include "pwrmgr.h"
#include "rf_phy_driver.h"

extern uint8_t  g_bootFlag ;
extern bool otp_go_read(void);
extern int pwrmgr_register(MODULE_e mod, pwrmgr_Hdl_t sleepHandle, pwrmgr_Hdl_t wakeupHandle);

static void hw_otp_spif_config(void)
{
	if(g_bootFlag)
	{
	  	otp_go_read();
	}
	else
	{
	    gpio_fmux_set(P35,FMUX_MISO_0);
	    gpio_fmux_set(P36,FMUX_CLK);
	  	gpio_fmux_set(P37,FMUX_CSN); 
	  	gpio_fmux_set(P38,FMUX_MISO_1);    
	  	init_spif();
	}

}
int hal_otp_flash_init(void)
{
	if(g_system_clk == SYS_CLK_XTAL_16M)
		*(volatile int*)0x40000028 |= 0x10; //bypass bridge
	else
		*(volatile int*)0x40000028 &= 0xFFFFFFEF;
    return pwrmgr_register(MOD_SPIF, NULL,  hw_otp_spif_config);
}