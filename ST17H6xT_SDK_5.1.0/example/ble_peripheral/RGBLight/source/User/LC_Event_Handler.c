/**
 *	@file		LC_Event_Handler.c
 *	@author		YQ(1500861686)
 *	@date		01/20/2021
 *	@brief		IRQ handler.
 */

/*!
 *	@defgroup	LC_Event_Handler
 *	@brief		DO NOT put Interrupt code in XIP flash!!!
 *	@{*/

/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Event_Handler.h"

#include "LC_Key.h"
#include "LC_IR_Rec.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/
const uint8_t pin_map[GPIO_NUM] = 
{
	0,    //p0
	1,    //p1
	2,    //p2
	3,    //p3
	7,    //p7
	9,    //p9
	10,   //p10
	11,   //p11
	14,   //p14
	15,   //p15
	16,   //p16
	17,   //p17
	18,   //p18
	20,   //p20
	23,   //p23
	24,   //p24
	25,   //p25
	26,   //p26
	27,   //p27
	31,   //p31
	32,   //p32
	33,   //p33
	34,   //p34
};
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
extern	uint32_t s_gpio_wakeup_src[2];
/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/**
 *	@fn			LC_RGB_Valeu_Deal
 *	@brief		Callback of timer handler.
 *	@param[in]	evt		:IRQ event.
 *	@return		none.
 */
void	__attribute__((used))	LC_RGB_Valeu_Deal(uint8 evt)
{
	LC_RGBLight_Dynamic_Basic_Timer();
#if(LC_RGBLight_IR_Enable == 1)
	LC_IR_Analysis_100ns_Cnt++;
#endif	//	LC_RGBLight_IR_Enable
}
/**
 *	@fn			LC_IR_Analysis_Data
 *	@brief		analysis NEC,get user code and commond code.
 *	@param[in]	nTimeL		:interval of a negtivate and a posetive edge.	
 *	@return		none.
 */
void	__attribute__((used))	LC_IR_Analysis_Data(uint32	nTimeL)
{
	static	uint16	bit_cnt	=	0;

	if((nTimeL > 25) && (nTimeL < 50))
	{
		bit_cnt	=	0;
		LC_IR_Analysis_KeyValue	=	0xFFFFFFFF;
	}
	else
	{
		if(nTimeL < 19)
		{
			if(nTimeL > 10)
			{
				LC_IR_Analysis_KeyValue &= ~ BIT(bit_cnt);
			}
			if(bit_cnt < 32)
			{
				bit_cnt++;
			}
			if(bit_cnt == 32)
			{
				// osal_start_timerEx(LC_IR_Rec_TaskID, IR_REC_EVENT_LEVEL2, 10);
				osal_set_event(LC_IR_Rec_TaskID, IR_REC_EVENT_LEVEL2);
				bit_cnt	=	0;
			}
		}
	}

}
/*!
 *	@fn			LC_Key_Pin_IntHandler_Neg
 *	@brief		Callback of key Pin interrupt.
 *	@param[in]	pin		:pin of IR.
 *	@param[in]	type	:type of interrupe.
 *	@return		none.
 */
void	__attribute__((used))	LC_Key_Pin_IntHandler_Neg(GPIO_Pin_e pin, IO_Wakeup_Pol_e type)
{
	uint8_t pin_idx = pin_map[pin];
	uint32_t triggered = s_gpio_wakeup_src[pin_idx/32] & BIT(pin_idx%32);

	if(triggered)
	{
		LOG("wakeup0 %d\n", triggered);
		// gpioin_unregister(GPIO_KEY_NO1);
		// osal_start_timerEx(LC_Key_TaskID, KEY_SCANF_EVT, 20);
	}
	else
	{
		LOG("Int0 %d\n", triggered);
	}

}

void	__attribute__((used))	LC_Key_Pin_IntHandler_Pos(GPIO_Pin_e pin, IO_Wakeup_Pol_e type)
{
	uint8_t pin_idx = pin_map[pin];
	uint32_t triggered = s_gpio_wakeup_src[pin_idx/32] & BIT(pin_idx%32);
	if(triggered)
	{
		LOG("wakeup1 %d\n", triggered);
	}
	else
	{
		LOG("Int1 %d\n", triggered);
		// gpioin_unregister(GPIO_KEY_NO1);
		// osal_start_timerEx(LC_Key_TaskID, KEY_SCANF_EVT, 20);
	}

}

void	__attribute__((used))	pin_event_Neg_handler(GPIO_Pin_e pin, gpio_polarity_e type)
{
	uint8_t pin_idx = pin_map[pin];
	uint32_t triggered = s_gpio_wakeup_src[pin_idx/32] & BIT(pin_idx%32);
    switch (pin)
    {
		#if (LC_RGBLight_Key_Enable == 1)
		case GPIO_KEY_NO1:
			LOG("KEY interrupt neg %d, src %d\n", type, triggered);
			break;
		#endif
		
		default:

			break;
    }
}
void	__attribute__((used))	pin_event_Pos_handler(GPIO_Pin_e pin, gpio_polarity_e type)
{
	uint8_t pin_idx = pin_map[pin];
	uint32_t triggered = s_gpio_wakeup_src[pin_idx/32] & BIT(pin_idx%32);
    switch (pin)
    {
		#if (LC_RGBLight_Key_Enable == 1)
		case GPIO_KEY_NO1:
				LOG("KEY interrupt pos %d, src %d\n", type, triggered);
			break;
		#endif

		default:

			break;
    }
}
/*!
 *	@fn			LC_Gpio_IR_IntHandler
 *	@brief		Callback of IR Pin interrupt.
 *	@param[in]	pin		:pin of IR.
 *	@param[in]	type	:type of interrupe.
 *	@return		none.
 */
void	__attribute__((used))	LC_Gpio_IR_IntHandler(GPIO_Pin_e pin, IO_Wakeup_Pol_e type)
{
	switch(pin)
	{
	#if(LC_RGBLight_IR_Enable == 1)
		case	GPIO_IR_REC:
			// LOG("ir int %d\n", type);
			if(type == NEGEDGE)
			{
				osal_set_event(LC_IR_Rec_TaskID, IR_REC_EVENT_LEVEL1);
			}
			else
			{
				LC_IR_Last_Posedge_Time	=	LC_IR_Analysis_100ns_Cnt;
			}

		break;
	#endif	//	LC_RGBLight_IR_Enable

		default:

		break;
	}
}

/** @}*/

