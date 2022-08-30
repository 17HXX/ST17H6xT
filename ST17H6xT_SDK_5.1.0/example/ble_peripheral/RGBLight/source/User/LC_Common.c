/**
*	@file	LC_Common.h
*	@author	YQ(1500861686)
*	@date	09/17/2020
*	@version	1.0.0
*
*/

/*!
 * 	@defgroup	LC_Common
 *	@brief
 *	@{*/
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Common.h"
#include "LC_Event_Handler.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/
static	uint8	LC_Timer_Working_Flag	=	State_Off;
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
lc_app_set_t		LC_App_Set_Param	=
{
	.app_write_data				=	{0},
	.app_notify_data			=	{0},
	.app_write_len				=	0,
	.app_notify_len				=	0,
};

lc_rgblight_t	LC_RGBLight_Param	=	
{
#if(LC_RGBLight_PWM_U16_Enable == 1)
	.RGB_rValue		=	2550,
	.RGB_gValue		=	0,
	.RGB_bValue		=	0,
#if(LC_RGBLight_Module == RGBWLight)
	.RGB_wValue		=	0,
	.RGB_wValue_New	=	0
#endif	//	LC_RGBLight_Module
	.RGB_rValue_New	=	2550,
	.RGB_gValue_New	=	0,
	.RGB_bValue_New	=	0,
#else
	.RGB_rValue		=	255,
	.RGB_gValue		=	0,
	.RGB_bValue		=	0,
#if(LC_RGBLight_Module == RGBWLight)
	.RGB_wValue		=	0,
	.RGB_wValue_New	=	0
#endif	//	LC_RGBLight_Module
	.RGB_rValue_New	=	255,
	.RGB_gValue_New	=	0,
	.RGB_bValue_New	=	0,
#endif	//	LC_RGBLight_PWM_U16_Enable
	.RGB_Speed_Reserved		=	0,
	.RGB_Mode_Change_Speed	=	50,
	.RGB_Mode_Change_Color_Num	=	0,
	.RGB_Mode_Fade_Color_Num	=	0,
	.RGB_Mode_Flash_Time_Num	=	0,
	.RGB_Light_State			=	State_On,
	.RGB_Light_Level			=	100,
	.RGB_Light_Mode				=	1,
	.RGB_Light_Mode_Reserved	=	0,
	.RGB_Light_Mode_Auto		=	0,
};
/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/*!
 *	@fn			clock_time_exceed_func
 *	@brief		
 */
uint32 clock_time_exceed_func(uint32 ref, uint32 span_ms)
{
#if 0
	u32 deltTick ,T0 ;
	T0 = hal_read_current_time();
	deltTick =TIME_DELTA(T0,ref);
	if(deltTick>span_ms){
		return 1 ;
	}else {
		return 0 ;
	}
#else 
	uint32 deltTick  = 0 ;
	deltTick = get_ms_intv(ref) ;
	if(deltTick>span_ms){
		return 1 ;
	}else {
		return 0 ;
	}	
#endif
}
/*!
 *	@fn			LC_Common_ProcessOSALMsg
 *	@brief		Process an incoming task message,nothing.
 *	@param[in]	pMsg	:message to process
 *	@return		none.
 */
void LC_Common_ProcessOSALMsg(osal_event_hdr_t *pMsg)
{
	switch(pMsg->event)
	{
		default:
			// do nothing
		break;
	}
}
void LC_Timer_Start(void)
{
	if(LC_Timer_Working_Flag == State_Off)
	{
		hal_timer_init(LC_RGB_Valeu_Deal);
		hal_timer_set(AP_TIMER_ID_4, 100);
		LC_Timer_Working_Flag	=	State_On;
	}
	else
	{
		return;
	}
}
void LC_Timer_Stop(void)
{
	if(LC_Timer_Working_Flag == State_On)
	{
		hal_timer_stop(AP_TIMER_ID_4);
		LC_Timer_Working_Flag	=	State_Off;
	}
	else
	{
		return;
	}
}
/** @}*/

