/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       bus_dev.h
  Revised:         
  Revision:        

  Description:    Describe the purpose and contents of the file.


 
**************************************************************************************************/

#ifndef _HAL_MCU_H
#define _HAL_MCU_H
 


/* ------------------------------------------------------------------------------------------------
 *                                           Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "types.h"
#include <stdint.h>

//enum{
//  MCU_UNDEF    = 0,
//  MCU_PRIME_A1 = 1,
//  MCU_PRIME_A2 = 2,
//  MCU_BUMBEE_M0,
//  MCU_BUMBEE_CK802
//};

#define MCU_UNDEF 0
#define MCU_PRIME_A1 1
#define MCU_PRIME_A2 2
#define MCU_BUMBEE_M0 3
#define MCU_BUMBEE_CK802 4


#define SRAM_BASE_ADDR          0x1fff0000
#define SRAM_END_ADDR           0x1fff7fff

#define ROM_SRAM_JUMPTABLE      SRAM_BASE_ADDR
#define ROM_SRAM_GLOBALCFG      (ROM_SRAM_JUMPTABLE+0x100)
#define ROM_SRAM_JUMPTABLE_MIRROR   0x1fff5000
#define ROM_SRAM_GLOBALCFG_MIRROR   (ROM_SRAM_JUMPTABLE_MIRROR+0x100)

#define ROM_SRAM_HEAP           0x1fff6000
#define ROM_SRAM_HEAP_SIZE      (1024*8)
#define ROM_SRAM_DWC_BUF        0x1fff7c00


#define APP_SRAM_START_ADDR     0x1fff2000


/* ------------------------------------------------------------------------------------------------
 *                                        Target Defines
 * ------------------------------------------------------------------------------------------------
 */
 
#define MAXMEMHEAP 4096

#define HAL_ISER   *((volatile uint32_t *)(0xe000e100))
#define HAL_ICER   *((volatile uint32_t *)(0xe000e180))


//#ifdef NO_STACK
//delete this to removing compile error of testcase31_lltest.
//#else

//subWriteReg: write value to register zone: bit[high:low]


#define   subWriteReg(addr,high,low,value)    {write_reg(addr,(read_reg(addr)&\
                                              (~((((unsigned int)1<<((high)-(low)+1))-1)<<(low))))|\
                                                  ((unsigned int)(value)<<(low)));__asm volatile("nop");}
//#endif
 
																										
#define TIME_BASE               (0x003fffff) // 24bit count shift 2 bit as 1us/bit
#define TIME_DELTA(x,y)         ( (x>=y) ? x-y : TIME_BASE-y+x )
																									
/*------------------------------------------------------
*  add arg for drv_enable_irq suport YoC software tspend
*/
//extern void drv_irq_init(void);
//extern int drv_enable_irq(void);
//extern int drv_disable_irq(void);
//#define HAL_CRITICAL_SECTION_INIT()   drv_irq_init()
//#define HAL_ENTER_CRITICAL_SECTION()  drv_disable_irq()
//#define HAL_EXIT_CRITICAL_SECTION()   drv_enable_irq()

/*-----------------------------------------------------
* for rom symoble use the _HAL_CS_ALLOC_
*/
extern void drv_irq_init(void);
extern int drv_enable_irq(int cs);
extern int drv_disable_irq(void);
extern int drv_enable_irqx(int cs);
extern int drv_disable_irqx(void);
#define HAL_CRITICAL_SECTION_INIT()             drv_irq_init()


#if(__BUILD_DRV_IRQ_CS_ALLOC__)
//#warning" !!! HAS CS ALLOC !!!"
#define _HAL_CS_ALLOC_()                        int _drv_irq_cs

#define HAL_ENTER_CRITICAL_SECTION()            { _drv_irq_cs = drv_disable_irq();} 
#define HAL_EXIT_CRITICAL_SECTION()             { drv_enable_irq(_drv_irq_cs);} 

#define DRV_ENTER_CRITICAL_SECTION()            {drv_disable_irqx();}
#define DRV_EXIT_CRITICAL_SECTION()             {drv_enable_irqx(0);}

#else                       

//#ifndef FPGA_PERIPHERAL_TEST

//#define HAL_ENTER_CRITICAL_SECTION()            { ;} 
//#define HAL_EXIT_CRITICAL_SECTION()             { ;} 
//#define DRV_ENTER_CRITICAL_SECTION()            { ;}
//#define DRV_EXIT_CRITICAL_SECTION()             { ;}

//#else
	
#define _HAL_CS_ALLOC_()                        int _drv_irq_cs;

#define HAL_ENTER_CRITICAL_SECTION()            { _drv_irq_cs = drv_disable_irq();} 
#define HAL_EXIT_CRITICAL_SECTION()             { drv_enable_irq(_drv_irq_cs);} 

#define DRV_ENTER_CRITICAL_SECTION()            {drv_disable_irqx();}
#define DRV_EXIT_CRITICAL_SECTION()             {drv_enable_irqx(0);}

//#endif

#endif

/**************************************************************************************************
 */
#endif
