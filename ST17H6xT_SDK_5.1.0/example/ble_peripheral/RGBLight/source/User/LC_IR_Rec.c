/**
 *	@file		LC_IR_Rec.c
 *	@author		YQ(1500861686)
 *	@date		12/23/2020
 *	@version	1.1.2
 */

/*!
 *	@defgroup	LC_IR_Rec
 *	@brief
 *	@{*/
/*------------------------------------------------------------------*/
/* 				  head files include							 	*/
/*------------------------------------------------------------------*/
#include "LC_IR_Rec.h"
#include "LC_RGBLight_Mode.h"
#include "LC_UI_Led_Buzzer.h"
#include "LC_Event_Handler.h"

#if(LC_RGBLight_IR_Enable == 1)
/*------------------------------------------------------------------*/
/* 					 	local variables		 						*/
/*------------------------------------------------------------------*/


/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
const		uint8	LC_RGBLight_Mode_IR_Buffer[60]	=	{
	//	normal 24keys colors
	255,  0,  0,	  0,255,  0,	  0,  0,255,	180,180,180,
	255, 28,  0,	  0,115, 28,	 85,  0,227,	255,193,193,  
	255, 57,  0,	  0,115, 54,	140,  0,227,	255,193,193,
	255,143,  0,	  0,115,143,	196,  0,227,	135,206,255,
	255,227,  0,	  0,115,227,	255,  0,227,	135,206,255,
};
volatile	uint32		LC_IR_Analysis_100ns_Cnt	=	0;
volatile	uint32		LC_IR_Analysis_KeyValue		=	0;
volatile	uint32		LC_IR_Last_Posedge_Time		=	0;
uint8		LC_IR_Rec_TaskID;

/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/

/*!
 *	@fn			LC_Gpio_IR_Rec_Init
 *	@brief		Initialize the IR receiver pin.
 *	@param[in]	none.	
 *	@return		none.
 */
void	LC_Gpio_IR_Rec_Init(void)
{
	//	Input
	gpio_dir(GPIO_IR_REC, IE);
	gpio_pull_set(GPIO_IR_REC, STRONG_PULL_UP);
	gpioin_register(GPIO_IR_REC, LC_Gpio_IR_IntHandler, LC_Gpio_IR_IntHandler);
}
/*!
 *	@fn			LC_RGBLight_Mode_Static_IRKeyboard
 *	@brief		IR Keyboard color change. 
 *	@param[in]	key_num				:key value of keyboard color.
 *	@param[in]	rgb_data			:pointer of color.
 *	@param[in]	temp				:.
 *	@return		none.
 */
void	LC_RGBLight_Mode_Static_IRKeyboard(uint8 key_num, const uint8 *rgb_data)
{
	LC_RGBLight_Param.RGB_Light_Mode_Auto	=	State_Off;
	LC_RGBLight_Param.RGB_Light_Mode	=	RGB_Plate_Mode;
	LC_RGBLight_Param.RGB_rValue_New	=	(*(rgb_data + key_num*3));
	LC_RGBLight_Param.RGB_gValue_New	=	(*(rgb_data + key_num*3 + 1));
	LC_RGBLight_Param.RGB_bValue_New	=	(*(rgb_data + key_num*3 + 2));
#if(LC_RGBLight_Module == RGBWLight)
	LC_RGBLight_Param.RGB_wValue_New	=	0;
	LC_RGBLight_Param.RGB_wValue		=	0;
#endif
	LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_PWMSetRGBValue();
}
/*!
 *	@fn			LC_IR_Rec_Task_Init 
 *	@brief		Initialize function for the LC_IR_Rec Task. 
 *	@param[in]	task_id			:the ID assigned by OSAL,
 *								used to send message and set timer.
 *	@retrurn	none.
 */
void	LC_IR_Rec_Task_Init(uint8 task_id)
{
	LC_IR_Rec_TaskID	=	task_id;
	LC_Gpio_IR_Rec_Init();
	LOG("IR receive pin init\n");
}
/*!
 *	@fn			LC_IR_Rec_ProcessEvent
 *	@brief		LC_IR_Rec Task event processor.This function
 *				is called to processs all events for the task.Events
 *				include timers,messages and any other user defined events.
 *	@param[in]	task_id			:The OSAL assigned task ID.
 *	@param[in]	events			:events to process.This is a bit map and can
 *									contain more than one event.
 */
uint16	LC_IR_Rec_ProcessEvent(uint8 task_id, uint16 events)
{
	VOID task_id;	// OSAL required parameter that isn't used in this function
	if(events & SYS_EVENT_MSG){
		uint8	*pMsg;
		if((pMsg = osal_msg_receive(LC_IR_Rec_TaskID)) != NULL){
			LC_Common_ProcessOSALMsg((osal_event_hdr_t *)pMsg);
            // Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}
		return(events ^ SYS_EVENT_MSG);
	}

	if(events & IR_REC_EVENT_LEVEL1)
	{
		LC_IR_Analysis_Data(LC_IR_Analysis_100ns_Cnt - LC_IR_Last_Posedge_Time);
		return(events^IR_REC_EVENT_LEVEL1);
	}


	if(events & IR_REC_EVENT_LEVEL2)
	{
		uint8	LC_IR_Keyboard_Num		=	0; 
		uint16	LC_IR_Keyboard_UserNum	=	0;
		
		LC_IR_Keyboard_Num		=	(uint8)((LC_IR_Analysis_KeyValue >> 24) & 0x000000ff);
		LC_IR_Keyboard_UserNum	=	(uint16)(LC_IR_Analysis_KeyValue & 0x0000ffff);
		LC_IR_Analysis_KeyValue =	0;
		LOG("ir analysis key %8x %8x\n",LC_IR_Keyboard_UserNum, LC_IR_Keyboard_Num);
		if(LC_IR_Keyboard_UserNum == IR_KeyBoard_Type_24Keys)
		{
			// RGBLight Off
			if((LC_RGBLight_Param.RGB_Light_State	== State_On) && (LC_IR_Keyboard_Num == IRKey_Light_Off))
			{
				LC_RGBLight_Turn_Onoff(State_Off);
			}
			//	RGBLight On
			else if((LC_RGBLight_Param.RGB_Light_State == State_Off) && (LC_IR_Keyboard_Num == IRKey_Light_On))
			{
				LC_RGBLight_Turn_Onoff(State_On);
			}
			
			if(LC_RGBLight_Param.RGB_Light_State == State_On)
			{
				if(LC_RGBLight_Param.RGB_Light_Mode == RGB_Plate_Mode)
				{
					//	RGBLight Level Up
					if(LC_IR_Keyboard_Num == IRKey_Light_Level_Up)
					{
						if(LC_RGBLight_Param.RGB_Light_Level < 100)
						{
							LC_RGBLight_Param.RGB_Light_Level	+=	10;
						}
						else
						{
							LC_RGBLight_Param.RGB_Light_Level	=	100;
						}
						LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
						LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
						LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
					#if(LC_RGBLight_Module == RGBWLight)
						LC_RGBLight_Param.RGB_wValue		=	0;
					#endif	//	LC_RGBLight_Module
						LC_PWMSetRGBValue();
					}
					//	RGBLight Level Down
					else if(LC_IR_Keyboard_Num == IRKey_Light_Level_Down)
					{
						if(LC_RGBLight_Param.RGB_Light_Level > 20)
						{
							LC_RGBLight_Param.RGB_Light_Level	-=	10;
						}
						else
						{
							LC_RGBLight_Param.RGB_Light_Level	=	10;
						}
						LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
						LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
						LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
					#if(LC_RGBLight_Module == RGBWLight)
						LC_RGBLight_Param.RGB_wValue		=	0;
					#endif	//	LC_RGBLight_Module
						LC_PWMSetRGBValue();
					}
				}
				else if(LC_RGBLight_Param.RGB_Light_Mode > RGB_Static_White)
				{
					if(LC_IR_Keyboard_Num == IRKey_Light_Level_Up)
					{
						if(LC_RGBLight_Param.RGB_Speed_Reserved < 100)
						{
							LC_RGBLight_Param.RGB_Speed_Reserved		+=	10;
						}
						else
						{
							LC_RGBLight_Param.RGB_Speed_Reserved		=	100;
						}
						LC_RGBLight_Param.RGB_Mode_Change_Speed 	=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
					}
					else if(LC_IR_Keyboard_Num == IRKey_Light_Level_Down)
					{
						if(LC_RGBLight_Param.RGB_Speed_Reserved > 10)
						{
							LC_RGBLight_Param.RGB_Speed_Reserved		-=	10;
						}
						else
						{
							LC_RGBLight_Param.RGB_Speed_Reserved		=	0;
						}
						LC_RGBLight_Param.RGB_Mode_Change_Speed 	=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
					}
				}				
				//	RGBLight Color Choose
				if((LC_IR_Keyboard_Num >= IRKey_Light_Red) && (LC_IR_Keyboard_Num <= IRKey_Mode_Smooth))
				{
				#if(LC_RGBLight_Mic_Enable == 1)
					LC_Mic_Sample_Stop();
				#endif	//	LC_RGBLight_Mic_Enable
					if(LC_IR_Keyboard_Num == IRKey_Mode_Flash)
					{
						LC_RGBLight_Modetick	=	0;
						LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
						LC_RGBLight_Param.RGB_Light_Mode_Auto		=	State_Off;
						LC_RGBLight_Param.RGB_Light_Mode			=	RGB_Flash_SevenColors;
						LC_RGBLight_Param.RGB_Speed_Reserved		=	20;
						LC_RGBLight_Param.RGB_Mode_Change_Speed 	=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
					}
					else if(LC_IR_Keyboard_Num	== IRKey_Mode_Strobe)
					{
						LC_RGBLight_Modetick	=	0;
						LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
						LC_RGBLight_Param.RGB_Light_Mode_Auto		=	State_Off;
						LC_RGBLight_Param.RGB_Light_Mode			=	RGB_Jump_SevenColors;
						LC_RGBLight_Param.RGB_Speed_Reserved		=	30;
						LC_RGBLight_Param.RGB_Mode_Change_Speed 	=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
					}
					else if(LC_IR_Keyboard_Num == IRKey_Mode_Fade)
					{
						LC_RGBLight_Modetick	=	0;
						LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
						LC_RGBLight_Param.RGB_Light_Mode_Auto		=	State_Off;
						LC_RGBLight_Param.RGB_Light_Mode			=	RGB_Fade_SevenColors;
						LC_RGBLight_Param.RGB_Speed_Reserved		=	100;
						LC_RGBLight_Param.RGB_Mode_Change_Speed 	=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
					}
					else if(LC_IR_Keyboard_Num == IRKey_Mode_Smooth)
					{
						LC_RGBLight_Modetick	=	0;
						LC_RGBLight_Param.RGB_Mode_Change_Color_Num =	0;
						LC_RGBLight_Param.RGB_Light_Mode_Auto		=	State_Off;
						LC_RGBLight_Param.RGB_Light_Mode			=	RGB_Smooth;
						LC_RGBLight_Param.RGB_Speed_Reserved		=	100;
						LC_RGBLight_Param.RGB_Mode_Change_Speed 	=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
					}
					else
					{
						LC_RGBLight_Mode_Static_IRKeyboard(LC_IR_Keyboard_Num - IRKey_Light_Red, LC_RGBLight_Mode_IR_Buffer);
						// LOG("IR choose color %d %d %d\n",LC_RGBLight_Param.RGB_rValue,LC_RGBLight_Param.RGB_gValue,LC_RGBLight_Param.RGB_bValue);
					}
					LC_RGBLight_Reserve_Mode();
				}
			}
		}
		return(events ^ IR_REC_EVENT_LEVEL2);
	}


    // Discard unknown events
	return 0;
}

#endif	//	LC_RGBLight_IR_Enable
/** @}*/

