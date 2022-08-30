/**
*	@file	LC_Common.h
*	@author	YQ(1500861686)
*	@date	10/20/2020
*	@version	1.0.1
*
*/

/*!
 * 	@defgroup	LC_Common
 *	@brief
 *	@{*/
#ifndef		LC_COMMON_H_
#define		LC_COMMON_H_
/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef	__cplusplus
	 extern  "C" {
#endif
/*------------------------------------------------------------------*/
/* 				 		head files include 							*/
/*------------------------------------------------------------------*/

#include "att.h"
#include "bcomdef.h"
#include "gapbondmgr.h"
#include "gapgattserver.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "global_config.h"
#include "hci.h"
#include "hci_tl.h"
#include "linkdb.h"
#include "ll.h"
#include "ll_def.h"
#include "ll_hw_drv.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "OSAL_Clock.h"
#include "pwm.h"
#include "pwrmgr.h"
#include "rf_phy_driver.h"
#include "osal_snv.h"
#include "gpio.h"
#include "watchdog.h"
#include "adc.h"

#include "simpleGATTprofile.h"
/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
//	Light LED Module option
#define		RGBLight						1
#define		RGBWLight						2

// <<< Use Configuration Wizard in Context Menu >>>

//	<h>		Device Module

//=================================================================
//	<q>		LC_RGBLight_Key_Enable	-	Key Enable Flag
#ifndef		LC_RGBLight_Key_Enable
#define		LC_RGBLight_Key_Enable			1
#endif

//	<q>		LC_RGBLight_IR_Enable	-	IR Enable Flag
#ifndef		LC_RGBLight_IR_Enable
#define		LC_RGBLight_IR_Enable			1
#endif

//	<q>		LC_RGBLight_Mic_Enable	-	Mic Enable Flag
#ifndef		LC_RGBLight_Mic_Enable
#define		LC_RGBLight_Mic_Enable			0
#endif

//	<q>		LC_EEPROM_Enable		-	EEPROM 24C02 Enable Flag
#ifndef		LC_EEPROM_Enable
#define		LC_EEPROM_Enable				1
#endif

//	</h>
//=================================================================

//	<h>		RGB LED Type Option

//=================================================================
//	<o>		LC_RGBLight_Module	-	Choose LED Type:RGB or RGBW,difference is CCT.
//	<i>		- RGBLight	-	RGB Color,Defaulr Type.
//	<i>		- RGBWLight	-	RGBW Color,
//	<1=>	RGBLight
//	<2=>	RGBWLight
#ifndef		LC_RGBLight_Module
#define		LC_RGBLight_Module				1
#endif

//	</h>
//=================================================================

//	<h>		RGB Light PWM Maximum Value

//=================================================================
//	<q>		LC_RGBLight_PWM_U16_Enable	-	PWM use uint8 or uint16
#ifndef		LC_RGBLight_PWM_U16_Enable
#define		LC_RGBLight_PWM_U16_Enable		0
#endif

// <<< end of configuration section >>>

/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/
//	Red Green Blue or White Pins
#if (LC_RGBLight_Module == RGBLight)
#define		GPIO_LED_R		P34
#define		GPIO_LED_G		P2
#define		GPIO_LED_B		P3
#elif (LC_RGBLight_Module == RGBWLight)
#define		GPIO_LED_W		P11
#endif

//	Keys
#ifdef	LC_RGBLight_Key_Enable
#if(LC_RGBLight_Key_Enable == 1)
#define		GPIO_KEY_NO1	P11
#define		GPIO_KEY_NO2	P14
#define		GPIO_KEY_NO3	P15
#endif
#endif

//	IR Pin
#ifdef	LC_RGBLight_IR_Enable
#if(LC_RGBLight_IR_Enable == 1)
#define		GPIO_IR_REC		P7
#endif
#endif

//	MIC	Pin
#ifdef	LC_RGBLight_Mic_Enable
#if(LC_RGBLight_Mic_Enable == 1)

#endif
#endif

//	EEPROM-24C02
#ifdef	LC_EEPROM_Enable
#if(LC_EEPROM_Enable == 1)
#define		GPIO_IIC_SDA	P33
#define		GPIO_IIC_SCL	P32
#define		LC_EEPROM_IIC_ID	0xA0
#endif
#endif
/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/


/*------------------------------------------------------------------*/
/* 					 	Data structures							 	*/
/*------------------------------------------------------------------*/
typedef uint8_t				u8;
typedef uint16_t			u16;
typedef uint32_t			u32;
typedef signed   char		int8_t;			//!< Signed 8 bit integer
typedef unsigned char		uint8_t;		//!< Unsigned 8 bit integer
typedef signed   short		int16_t;		//!< Signed 16 bit integer
typedef unsigned short		uint16_t;		//!< Unsigned 16 bit integer
typedef signed   int		int32_t;		//!< Signed 32 bit integer
typedef unsigned int		uint32_t;		//!< Unsigned 32 bit integer
typedef signed   char		int8;			//!< Signed 8 bit integer
typedef unsigned char		uint8;			//!< Unsigned 8 bit integer
typedef signed   short		int16;			//!< Signed 16 bit integer
typedef unsigned short		uint16;			//!< Unsigned 16 bit integer
typedef signed   long		int32;			//!< Signed 32 bit integer
typedef unsigned long		uint32;			//!< Unsigned 32 bit integer

typedef		enum	
{
	LC_DEV_BLE_DISCONNECTION	=	0,
	LC_DEV_BLE_CONNECTION,
}lc_dev_ble_state;

typedef		enum	
{
	State_Off	=	0,
	State_On	=	1,
}lc_state_bool;

typedef		enum	{
	SYSTEM_POWEROFF	=	0,	SYSTEM_STANDBY	=	0,
	SYSTEM_WORKING	=	1,
	SYSTEM_SUSPEND	=	2,
}lc_sys_run_t;

typedef	struct
{
	uint8		app_write_data[20];
	uint8		app_notify_data[20];
	uint8		app_write_len;
	uint8		app_notify_len;
}lc_app_set_t;

typedef struct
{
#if(LC_RGBLight_PWM_U16_Enable == 1)
	uint16		RGB_rValue;					//	value write in register = value_new*light_level/10(0-2550)
	uint16		RGB_gValue;
	uint16		RGB_bValue;
#if(LC_RGBLight_Module == RGBWLight)
	uint16		RGB_wValue;
	uint16		RGB_wValue_New;
#endif	//	LC_RGBLight_Module
	uint16		RGB_rValue_New;				//	renew color value(0-255)from app & ir keyboard		
	uint16		RGB_gValue_New;
	uint16		RGB_bValue_New;
#else	//	LC_RGBLight_PWM_U16_Enable
	uint8		RGB_rValue;					//	value write in register(0 - 255)
	uint8		RGB_gValue;
	uint8		RGB_bValue;
#if(LC_RGBLight_Module == RGBWLight)
	uint8		RGB_wValue;
	uint8		RGB_wValue_New;
#endif	//	LC_RGBLight_Module
	uint8		RGB_rValue_New;				//	renew color value(0-255)from app & ir keyboard		
	uint8		RGB_gValue_New;
	uint8		RGB_bValue_New;
#endif	//	LC_RGBLight_PWM_U16_Enable
	uint16		RGB_Speed_Reserved;			//	original value of speed:0 - 100
	uint16		RGB_Mode_Change_Speed;		//	speed of mode
	uint8		RGB_Mode_Change_Color_Num;	//	number of color in dynamic mode
	uint8		RGB_Mode_Fade_Color_Num;	//	sequence of color in fade mode
	uint8		RGB_Mode_Flash_Time_Num;	//	flash times of one mode
	uint8		RGB_Light_State;			//	on	1,off	0
	uint8		RGB_Light_Color_Sequence;	//	default	0		1		2		3		4		5
											//		(R G B)	(R B G)	(G R B)	(G B R)	(B R G)	(B G R)
	uint8		RGB_Light_Level;			//	level of lightness:0--100
	uint8		RGB_Light_Mode;				//	static mode,	dynamic mode
	uint8		RGB_Light_Mode_Reserved;	//	reserved mode before turn off
	uint8		RGB_Light_Mode_Auto;		//	on	1,off	0
	uint8		RGB_Mic_Mode;				//	01:classical;02:soft;03:beats;04:disco
	uint8		RGB_Mic_Sensitivity;		//	Sensitivity of Mic:0--100
}lc_rgblight_t;

/*------------------------------------------------------------------*/
/* 					 external variables							 	*/
/*------------------------------------------------------------------*/
extern	lc_rgblight_t		LC_RGBLight_Param;
extern	lc_app_set_t		LC_App_Set_Param;
/*------------------------------------------------------------------*/
/* 					 User function prototypes					 	*/
/*------------------------------------------------------------------*/
extern	uint32	clock_time_exceed_func			(uint32 ref,		uint32 span_ms	);
extern	void	LC_Common_ProcessOSALMsg	 	(osal_event_hdr_t *pMsg				);
extern	void	LC_Timer_Start					(void);
extern	void	LC_Timer_Stop					(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LC_COMMON_H_ */
/** @}*/
