/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/



#ifndef _DFL_H_
#define _DFL_H_


#include "bus_dev.h"
#include "mcu.h"


extern uint32_t DFL_ENTRY_BASE;

#define DFL_FUNC(id) (*((uint32_t*)(DFL_ENTRY_BASE + 16 + id*4)))






int dfl_init(uint32_t dfp_addr, uint32_t run_addr, uint32_t run_size);
uint8_t dfl_num(void);
uint8_t dfl_current(void);
int dfl_load(uint8_t dfp_index);


#endif
