/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/



#include "rom_sym_def.h"
#include "bus_dev.h"
#include "gpio.h"
#include "clock.h"
#include "timer.h"
#include "jump_function.h"
#include "pwrmgr.h"
#include "mcu.h"
#include "gpio.h"
#include "log.h"
#include "rf_phy_driver.h"
#include "spif.h"
#include "otp.h"
#include "version.h"
#include "watchdog.h"

/*********************************************************************
    LOCAL FUNCTION PROTOTYPES
*/

/*********************************************************************
    EXTERNAL FUNCTIONS
*/

extern void init_config(void);
extern int app_main(void);
extern void hal_rom_boot_init(void);



/*********************************************************************
    OSAL LARGE HEAP CONFIG
*/
#define     LARGE_HEAP_SIZE  (2*1024)
uint8      g_largeHeap[LARGE_HEAP_SIZE] __attribute__ ((aligned(4)));

/*********************************************************************
    GLOBAL VARIABLES
*/
volatile uint8 g_clk32K_config;

/*********************************************************************
    EXTERNAL VARIABLES
*/
extern uint32_t  __initial_sp;



static void hal_low_power_io_init(void)
{
    //========= pull all io to gnd by default
    ioinit_cfg_t ioInit[]=
    {

        {GPIO_P02,   GPIO_FLOATING   },/*SWD*/
        {GPIO_P03,   GPIO_FLOATING   },/*SWD*/
        {GPIO_P09,   GPIO_PULL_UP    },/*UART TX*/
        {GPIO_P10,   GPIO_PULL_UP    },/*UART RX*/
        {GPIO_P11,   GPIO_PULL_DOWN  },
        {GPIO_P14,   GPIO_PULL_DOWN  },
        {GPIO_P15,   GPIO_PULL_DOWN  },
        {GPIO_P16,   GPIO_FLOATING   },
        {GPIO_P18,   GPIO_PULL_DOWN  },
        {GPIO_P20,   GPIO_PULL_DOWN  },
        #if(SDK_VER_CHIP==__DEF_CHIP_QFN32__)

        {GPIO_P00,   GPIO_PULL_DOWN  },
        {GPIO_P01,   GPIO_PULL_DOWN  },
        {GPIO_P07,   GPIO_PULL_DOWN  },
        {GPIO_P17,   GPIO_FLOATING   },/*32k xtal*/
        {GPIO_P23,   GPIO_PULL_DOWN  },
        {GPIO_P24,   GPIO_PULL_DOWN  },
        {GPIO_P25,   GPIO_PULL_DOWN  },
        {GPIO_P26,   GPIO_PULL_DOWN  },
        {GPIO_P27,   GPIO_PULL_DOWN  },
        {GPIO_P31,   GPIO_PULL_DOWN  },
        {GPIO_P32,   GPIO_PULL_DOWN  },
        {GPIO_P33,   GPIO_PULL_DOWN  },
        {GPIO_P34,   GPIO_PULL_DOWN  },
        #endif
    };

    for(uint8_t i=0; i<sizeof(ioInit)/sizeof(ioinit_cfg_t); i++)
        gpio_pull_set(ioInit[i].pin,ioInit[i].type);

    DIG_LDO_CURRENT_SETTING(0x01);
    pwrmgr_RAM_retention_set();
    pwrmgr_LowCurrentLdo_enable();
}

static void ble_mem_init_config(void)
{
    osal_mem_set_heap((osalMemHdr_t*)g_largeHeap, LARGE_HEAP_SIZE);
}

static void hal_rfphy_init(void)
{
    //============config the txPower
    g_rfPhyTxPower  = RF_PHY_TX_POWER_0DBM ;
    //============config BLE_PHY TYPE
    g_rfPhyPktFmt   = PKT_FMT_BLE1M;
    //============config RF Frequency Offset
    g_rfPhyFreqOffSet   =RF_PHY_FREQ_FOFF_00KHZ;
    //============config xtal 16M cap
    XTAL16M_CAP_SETTING(0x09);
    XTAL16M_CURRENT_SETTING(0x03);
    hal_rom_boot_init();
    NVIC_SetPriority((IRQn_Type)BB_IRQn,    IRQ_PRIO_REALTIME);
    NVIC_SetPriority((IRQn_Type)TIM1_IRQn,  IRQ_PRIO_HIGH);     //ll_EVT
    NVIC_SetPriority((IRQn_Type)TIM2_IRQn,  IRQ_PRIO_HIGH);     //OSAL_TICK
    //ble memory init and config
    ble_mem_init_config();
}

static void hal_init(void)
{
    hal_low_power_io_init();
    clk_init(g_system_clk); //system init
    hal_rtc_clock_config((CLK32K_e)g_clk32K_config);

    hal_pwrmgr_init();
    //TODO
    /*
        add opt and flash init 
    */
    LOG_INIT();

    gpio_init();
}

int  main(void)  
{     
    //hal_watchdog_config(WDG_2S);
    g_system_clk = SYS_CLK_XTAL_16M;//SYS_CLK_DBL_32M,SYS_CLK_XTAL_16M;
    g_clk32K_config = CLK_32K_RCOSC;//CLK_32K_XTAL,CLK_32K_RCOSC

    drv_irq_init();
    init_config();
    hal_rfphy_init();
    hal_init();

    if(gpio_read(P20)==1)
	{
        //rf_phy_direct_test();
	}
	
    LOG("SDK Version ID %08x \n",SDK_VER_RELEASE_ID);
    LOG("rfClk %d rcClk %d sysClk %d tpCap[%02x %02x]\n",g_rfPhyClkSel,g_clk32K_config,g_system_clk,g_rfPhyTpCal0,g_rfPhyTpCal1);
    LOG("[REST CAUSE] %d\n ",g_system_reset_cause);
    app_main();
}


///////////////////////////////////  end  ///////////////////////////////////////

