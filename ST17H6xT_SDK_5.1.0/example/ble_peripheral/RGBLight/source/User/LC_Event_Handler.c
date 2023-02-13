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
	{
		LOG("Int0 %d\n", triggered);
	}

}

void	__attribute__((used))	LC_Key_Pin_IntHandler_Pos(GPIO_Pin_e pin, IO_Wakeup_Pol_e type)
{
	{
		LOG("Int1 %d\n", triggered);
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

