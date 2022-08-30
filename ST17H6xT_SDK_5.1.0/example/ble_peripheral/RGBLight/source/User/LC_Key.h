/**
*	@file	LC_Key.h
*	@author	YQ(1500861686)
*	@date	09/17/2020
*	@version	1.0.0
*
*/

/*!
 * 	@defgroup	LC_Key
 *	@brief
 *	@{*/

#ifndef LC_KEY_H_
#define LC_KEY_H_

/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Common.h"

#if(LC_RGBLight_Key_Enable == 1)
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/
#define		KEY_SCANF_EVT		0x0001
#define		KEY_STOPSCANF_EVT	0x0002
#define		KEY_EVENT_LEVEL1	0x0004
#define		KEY_EVENT_SCANF		0x0008

#define		CHARGING_START_EVT	0x0010
#define		CHARGING_CHECK_EVT	0x0020
/*------------------------------------------------------------------*/
/*						Data structures								*/
/*------------------------------------------------------------------*/
typedef struct
{
	uint32 key_down_sys_tick; //	system time when key down
	uint8 key_down_flag;      //	1:key down	0:key release
	uint8 key_repeated_num;   //	times of key down
} lc_key_struct_data;

/*------------------------------------------------------------------*/
/*						external variables							*/
/*------------------------------------------------------------------*/
extern uint8 LC_Key_TaskID;
extern lc_key_struct_data LC_Key_Param;
/*------------------------------------------------------------------*/
/*						User function prototypes					*/
/*------------------------------------------------------------------*/
extern	void	LC_Key_Gpio_Init			(void);
extern	void	LC_Key_Task_Init			(uint8 task_id);
extern	uint16	LC_Key_ProcessEvent			(uint8 task_id, uint16 events);
extern	void	LC_Key_Pin_IntHandler		(GPIO_Pin_e pin, IO_Wakeup_Pol_e type);

#ifdef __cplusplus
}
#endif

#endif	//	LC_RGBLight_Key_Enable

#endif /* LC_KEY_H_ */
/** @}*/
