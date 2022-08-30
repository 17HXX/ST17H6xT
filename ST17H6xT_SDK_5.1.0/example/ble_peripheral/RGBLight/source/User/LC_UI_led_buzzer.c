/**
 *	@file		LC_UI_led_buzzer.h
 *	@author		YQ(1500861686)
 *	@date		09/16/2020
 *	@version	1.2.0
 *
 */

/*!
 * 	@defgroup	LC_UI_led_buzzer
 *	@brief
 *	@{*/

/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_UI_Led_Buzzer.h"
#include "LC_Key.h"
#include "LC_RGBLight_Mode.h"
#include "LC_RTC8563.h"
#include "LC_IIC_Dev.h"
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint8	LC_Ui_Led_Buzzer_TaskID;
gpioin_t	pin_test[3];
/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/
/**
 * @brief 	Read RGB data from flash.
 * 
 */
static	void	LC_ReadReservedData(void)
{
	uint8	EEPROM_Read_Buffer[32];

	AT24CXX_Read(EEPROM_RGB_MODE_DATA, EEPROM_Read_Buffer, 16);
	WaitMs(2);
	AT24CXX_Read(EEPROM_RGB_ALARM_DATA, &EEPROM_Read_Buffer[16], 16);
	LOG("read flash mode\n");
	LOG_DUMP_BYTE(EEPROM_Read_Buffer,16);
	LOG("read flash alarm\n");
	LOG_DUMP_BYTE(EEPROM_Read_Buffer+16,16);

	if(EEPROM_Read_Buffer[0] == 0x55)
	{
		LC_RGBLight_Param.RGB_Light_Mode	=	EEPROM_Read_Buffer[1];
		LC_RGBLight_Param.RGB_Light_Color_Sequence	=	EEPROM_Read_Buffer[14];
		if((LC_RGBLight_Param.RGB_Light_Mode == RGB_Plate_Mode) || \
			((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Static_Red) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Static_White)))
		{
		#if(LC_RGBLight_PWM_U16_Enable == 1)
			LC_RGBLight_Param.RGB_rValue	=	(uint16)((EEPROM_Read_Buffer[2] << 8)&0xff00) + EEPROM_Read_Buffer[3];
			LC_RGBLight_Param.RGB_gValue	=	(uint16)((EEPROM_Read_Buffer[4] << 8)&0xff00) + EEPROM_Read_Buffer[5];
			LC_RGBLight_Param.RGB_bValue	=	(uint16)((EEPROM_Read_Buffer[6] << 8)&0xff00) + EEPROM_Read_Buffer[7];
		#if(LC_RGBLight_Module == RGBWLight)
			LC_RGBLight_Param.RGB_wValue	=	(uint16)((EEPROM_Read_Buffer[8] << 8)&0xff00) + EEPROM_Read_Buffer[9];
		#endif	//	LC_RGBLight_Module
		#else
			LC_RGBLight_Param.RGB_rValue	=	EEPROM_Read_Buffer[3];
			LC_RGBLight_Param.RGB_gValue	=	EEPROM_Read_Buffer[5];
			LC_RGBLight_Param.RGB_bValue	=	EEPROM_Read_Buffer[7];
		#if(LC_RGBLight_Module == RGBWLight)
			LC_RGBLight_Param.RGB_wValue	=	EEPROM_Read_Buffer[9];
		#endif	//	LC_RGBLight_Module

		#endif	//	LC_RGBLight_PWM_U16_Enable
		}
		else if((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Fade_ThreeColors) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Smooth))
		{
			LC_RGBLight_Param.RGB_Mode_Change_Speed	=	(uint16)((EEPROM_Read_Buffer[12] << 8)&0xff00) + EEPROM_Read_Buffer[13];
		}
		else
		{
			LOG("mode err\n");
		}
	}

	if(EEPROM_Read_Buffer[16] == 0xaa)
	{
		LC_Dev_RTC8563_Alarm[0].Hour	=	EEPROM_Read_Buffer[17];
		LC_Dev_RTC8563_Alarm[0].Minute	=	EEPROM_Read_Buffer[18];
		LC_Dev_RTC8563_Alarm[0].Second	=	EEPROM_Read_Buffer[19];
		LC_Dev_RTC8563_Alarm[0].Func	=	EEPROM_Read_Buffer[20];
		LC_Dev_RTC8563_Alarm[0].Enable	=	EEPROM_Read_Buffer[21];
		LC_Dev_RTC8563_Alarm[0].WeekDay =	EEPROM_Read_Buffer[22];

		LC_Dev_RTC8563_Alarm[1].Hour	=	EEPROM_Read_Buffer[23];
		LC_Dev_RTC8563_Alarm[1].Minute	=	EEPROM_Read_Buffer[24];
		LC_Dev_RTC8563_Alarm[1].Second	=	EEPROM_Read_Buffer[25];
		LC_Dev_RTC8563_Alarm[1].Func	=	EEPROM_Read_Buffer[26];
		LC_Dev_RTC8563_Alarm[1].Enable	=	EEPROM_Read_Buffer[27];
		LC_Dev_RTC8563_Alarm[1].WeekDay =	EEPROM_Read_Buffer[28];
	}
}
/**
 * @brief 	Initlize PWM channel.No output before start PWM.
 * 
 */
static	void	LC_PWMChannelInit(void)
{
	pwm_init(PWM_CH0, PWM_CLK_NO_DIV, PWM_CNT_UP, PWM_POLARITY_FALLING);
	pwm_open_channel(PWM_CH0, GPIO_LED_R);
	
	pwm_init(PWM_CH1, PWM_CLK_NO_DIV, PWM_CNT_UP, PWM_POLARITY_FALLING);
	pwm_open_channel(PWM_CH1, GPIO_LED_G);
	
	pwm_init(PWM_CH2, PWM_CLK_NO_DIV, PWM_CNT_UP, PWM_POLARITY_FALLING);
	pwm_open_channel(PWM_CH2, GPIO_LED_B);
#if(LC_RGBLight_Module == RGBWLight)
	pwm_init(PWM_CH3, PWM_CLK_NO_DIV, PWM_CNT_UP, PWM_POLARITY_FALLING);
	pwm_open_channel(PWM_CH3, GPIO_LED_WW);
#endif
}
/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/**
 * @brief 	Initlize of Pins.
 * 
 */
void	LC_GPIO_RGBPinInit(void)
{
	gpio_dir(GPIO_LED_R, OEN);
	gpio_write(GPIO_LED_R, 0);

	gpio_fmux_control(GPIO_LED_G, Bit_DISABLE);
	gpio_dir(GPIO_LED_G, OEN);
	gpio_write(GPIO_LED_G, 0);
	
	gpio_fmux_control(GPIO_LED_B, Bit_DISABLE);
	gpio_dir(GPIO_LED_B, OEN);
	gpio_write(GPIO_LED_B, 0);

#if(LC_RGBLight_Module == RGBWLight)	
	gpio_dir(GPIO_LED_W, OEN);
	gpio_write(GPIO_LED_W, 0);
#endif
}
/**
 * @brief 	Set RGB value to PWM register.
 * 
 */
void	LC_PWMSetRGBValue(void)
{
	pwm_set_count_val(PWM_CH0, LC_RGBLight_Param.RGB_rValue, RGB_PWM_MAX);
	pwm_set_count_val(PWM_CH1, LC_RGBLight_Param.RGB_gValue, RGB_PWM_MAX);
	pwm_set_count_val(PWM_CH2, LC_RGBLight_Param.RGB_bValue, RGB_PWM_MAX);

#if(LC_RGBLight_Module == RGBWLight)
	pwm_set_count_val(PWM_CH3, LC_RGBLight_Param.RGB_wValue, RGB_PWM_MAX);
#endif

}

/*!
 *	@fn			LC_Gpio_UI_Led_Buzzer_Init
 *	@brief		Initialize the LED and Buzzer pins. 
 *	@param[in]	none.
 *	@return		none.
 */
void	LC_Gpio_UI_Led_Buzzer_Init(void)
{
	int ret;
	ret = gpioin_init(pin_test,sizeof(pin_test)/sizeof(pin_test[0]));
	if(ret != PPlus_SUCCESS)
	{
		LOG("gpio init error:%d\n",ret);
	}
	AT24CXX_Init();
	LC_GPIO_RGBPinInit();
	LC_ReadReservedData();
	LC_PWMChannelInit();
	LC_PWMSetRGBValue();
	pwm_start();
}

void	LC_RGBLight_Reserve_Mode(void)
{
	osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, SAVE_MODE_DATA_EVT, 500);
}

void	LC_RGBLight_Reserve_Alarm(void)
{
	osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, SAVE_ALARM_DATA_EVT, 500);
}

/*!
 *	@fn			LC_UI_Led_Buzzer_Task_Init 
 *	@brief		Initialize function for the UI_LED_BUZZER Task. 
 *	@param[in]	task_id			:the ID assigned by OSAL,
 *								used to send message and set timer.
 *	@retrurn	none.
 */
void	LC_UI_Led_Buzzer_Task_Init(uint8 task_id)
{
	LC_Ui_Led_Buzzer_TaskID	=	task_id;
	LOG("LC_Gpio_UI_Led_Buzzer_Init:\n");
    pwrmgr_register(MOD_USR8, NULL, NULL);
    pwrmgr_unlock(MOD_USR8);
	LC_Set_SysRtcTime(LC_RGB_DEFAULT_UNIX);
	LC_Timer_Start();

	LC_Gpio_UI_Led_Buzzer_Init();
	osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, DEAL_RTC_ALARM_EVT, 10);
}
/*!
 *	@fn			LC_UI_Led_Buzzer_ProcessEvent
 *	@brief		UI_LED_BUZZER Task event processor.This function
 *				is called to processs all events for the task.Events
 *				include timers,messages and any other user defined events.
 *	@param[in]	task_id			:The OSAL assigned task ID.
 *	@param[in]	events			:events to process.This is a bit map and can
 *									contain more than one event.
 */
uint16	LC_UI_Led_Buzzer_ProcessEvent(uint8 task_id, uint16 events)
{
	VOID task_id;	// OSAL required parameter that isn't used in this function
	if(events & SYS_EVENT_MSG){
		uint8	*pMsg;
		if((pMsg = osal_msg_receive(LC_Ui_Led_Buzzer_TaskID)) != NULL){
			LC_Common_ProcessOSALMsg((osal_event_hdr_t *)pMsg);
            // Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}
		return(events ^ SYS_EVENT_MSG);
	}

	if(events & DEAL_RTC_ALARM_EVT)
	{
		LC_Get_SysRtcTime();
		LC_RGBLight_Alarm_Process();
		osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, DEAL_RTC_ALARM_EVT, 500);
		return(events ^ DEAL_RTC_ALARM_EVT);
	}

	// deal with datas from APP
	if(events & DEAL_APP_DATA_EVT)
	{
	
		LOG("Decrype data: ");
		LOG_DUMP_BYTE(LC_App_Set_Param.app_write_data,LC_App_Set_Param.app_write_len);

		if((LC_App_Set_Param.app_write_data[0] == 0x7E) && (LC_App_Set_Param.app_write_data[8] == 0xEF))
		{
			if((LC_App_Set_Param.app_write_data[1] == 0x04) && (LC_App_Set_Param.app_write_data[2] == 0x04))
			{
				// RGBLight Off
				if((LC_App_Set_Param.app_write_data[5] == 0x00) && (LC_RGBLight_Param.RGB_Light_State == State_On))
				{
					LC_RGBLight_Turn_Onoff(State_Off);
				}
				//	RGBLight On
				else if((LC_App_Set_Param.app_write_data[5] == 0x01) && (LC_RGBLight_Param.RGB_Light_State == State_Off))
				{
					LC_RGBLight_Turn_Onoff(State_On);
				}
			}
			//	set UNIX time,app data do not contain year month and day,so the time_set commond is invalid
			else if((LC_App_Set_Param.app_write_data[1] == 0x07) && (LC_App_Set_Param.app_write_data[2] == 0x83))
			{
				LC_Dev_RTC8563_DateTime.dev_hour	=	LC_App_Set_Param.app_write_data[3];
				LC_Dev_RTC8563_DateTime.dev_minute	=	LC_App_Set_Param.app_write_data[4];
				LC_Dev_RTC8563_DateTime.dev_second	=	LC_App_Set_Param.app_write_data[5];
				if(LC_App_Set_Param.app_write_data[6] == 0)
				{
					LC_Dev_RTC8563_DateTime.dev_week_app	=	7;
				}
				else
				{
					LC_Dev_RTC8563_DateTime.dev_week_app	=	LC_App_Set_Param.app_write_data[6];
				}
				LC_Dev_RTC8563_DateTime.dev_week_old	=	LC_Dev_RTC8563_DateTime.dev_week;
				LC_Set_SysRtcTime(my_rtc_mktime(LC_Dev_RTC8563_DateTime));
			}
			//	set alarm time
			else if((LC_App_Set_Param.app_write_data[1] == 0x08) && (LC_App_Set_Param.app_write_data[2] == 0x82))
			{
				if(LC_App_Set_Param.app_write_data[6] == 0x00)
				{
					if((LC_App_Set_Param.app_write_data[3] == 0xff) && (LC_App_Set_Param.app_write_data[4] == 0xff) && (LC_App_Set_Param.app_write_data[5] == 0xff))
					{
						LC_App_Set_Param.app_write_data[3]	=	LC_Dev_RTC8563_Alarm[0].Hour;
						LC_App_Set_Param.app_write_data[4]	=	LC_Dev_RTC8563_Alarm[0].Minute;
						LC_App_Set_Param.app_write_data[5]	=	LC_Dev_RTC8563_Alarm[0].Second;
						LC_App_Set_Param.app_write_data[6]	=	0x00;
						LC_App_Set_Param.app_write_data[7]	=	(LC_Dev_RTC8563_Alarm[0].Enable << 7)^LC_Dev_RTC8563_Alarm[0].WeekDay;
						simpleGATTProfile_SetParamter(SIMPLEGATTPROFILE_CHAR1, 9, LC_App_Set_Param.app_write_data);
					}
					else
					{
						LC_Dev_RTC8563_Alarm[0].Hour	=	LC_App_Set_Param.app_write_data[3];
						LC_Dev_RTC8563_Alarm[0].Minute	=	LC_App_Set_Param.app_write_data[4];
						LC_Dev_RTC8563_Alarm[0].Second	=	LC_App_Set_Param.app_write_data[5];
						LC_Dev_RTC8563_Alarm[0].Func	=	LC_App_Set_Param.app_write_data[6];
						LC_Dev_RTC8563_Alarm[0].Enable	=	((LC_App_Set_Param.app_write_data[7] & 0x80) ? State_On : State_Off);
						LC_Dev_RTC8563_Alarm[0].WeekDay =	LC_App_Set_Param.app_write_data[7] & 0x7f;
						LC_RGBLight_Reserve_Alarm();
						// LOG("enable 0 %x\n",LC_Dev_RTC8563_Alarm[0].Enable);
					}
				}
				else if(LC_App_Set_Param.app_write_data[6] == 0x01)
				{
					if((LC_App_Set_Param.app_write_data[3] == 0xff) && (LC_App_Set_Param.app_write_data[4] == 0xff) && (LC_App_Set_Param.app_write_data[5] == 0xff))
					{
						LC_App_Set_Param.app_write_data[3]	=	LC_Dev_RTC8563_Alarm[1].Hour;
						LC_App_Set_Param.app_write_data[4]	=	LC_Dev_RTC8563_Alarm[1].Minute;
						LC_App_Set_Param.app_write_data[5]	=	LC_Dev_RTC8563_Alarm[1].Second;
						LC_App_Set_Param.app_write_data[6]	=	0x01;
						LC_App_Set_Param.app_write_data[7]	=	(LC_Dev_RTC8563_Alarm[1].Enable << 7)^LC_Dev_RTC8563_Alarm[1].WeekDay;
						simpleGATTProfile_SetParamter(SIMPLEGATTPROFILE_CHAR1, 9, LC_App_Set_Param.app_write_data);
					}
					else
					{
						LC_Dev_RTC8563_Alarm[1].Hour	=	LC_App_Set_Param.app_write_data[3];
						LC_Dev_RTC8563_Alarm[1].Minute	=	LC_App_Set_Param.app_write_data[4];
						LC_Dev_RTC8563_Alarm[1].Second	=	LC_App_Set_Param.app_write_data[5];
						LC_Dev_RTC8563_Alarm[1].Func	=	LC_App_Set_Param.app_write_data[6];
						LC_Dev_RTC8563_Alarm[1].Enable	=	((LC_App_Set_Param.app_write_data[7] & 0x80) ? State_On : State_Off);
						LC_Dev_RTC8563_Alarm[1].WeekDay	=	LC_App_Set_Param.app_write_data[7] & 0x7f;
						LC_RGBLight_Reserve_Alarm();
						// LOG("enable 1 %x\n",LC_Dev_RTC8563_Alarm[1].Enable);
					}
				}
			}
			//	set RGB color sequence
			else if((LC_App_Set_Param.app_write_data[1] == 0x06) && (LC_App_Set_Param.app_write_data[2] == 0x81))
			{
				if((LC_App_Set_Param.app_write_data[3] == 0x01) && (LC_App_Set_Param.app_write_data[4] == 0x02) && (LC_App_Set_Param.app_write_data[5] == 0x03))
				{
					LC_RGBLight_Param.RGB_Light_Color_Sequence	=	0;
				}
				else if((LC_App_Set_Param.app_write_data[3] == 0x01) && (LC_App_Set_Param.app_write_data[4] == 0x03) && (LC_App_Set_Param.app_write_data[5] == 0x02))
				{
					LC_RGBLight_Param.RGB_Light_Color_Sequence	=	1;
				}
				else if((LC_App_Set_Param.app_write_data[3] == 0x02) && (LC_App_Set_Param.app_write_data[4] == 0x01) && (LC_App_Set_Param.app_write_data[5] == 0x03))
				{
					LC_RGBLight_Param.RGB_Light_Color_Sequence	=	2;
				}
				else if((LC_App_Set_Param.app_write_data[3] == 0x02) && (LC_App_Set_Param.app_write_data[4] == 0x03) && (LC_App_Set_Param.app_write_data[5] == 0x01))
				{
					LC_RGBLight_Param.RGB_Light_Color_Sequence	=	3;
				}
				else if((LC_App_Set_Param.app_write_data[3] == 0x03) && (LC_App_Set_Param.app_write_data[4] == 0x01) && (LC_App_Set_Param.app_write_data[5] == 0x02))
				{
					LC_RGBLight_Param.RGB_Light_Color_Sequence	=	4;
				}
				else if((LC_App_Set_Param.app_write_data[3] == 0x03) && (LC_App_Set_Param.app_write_data[4] == 0x02) && (LC_App_Set_Param.app_write_data[5] == 0x01))
				{
					LC_RGBLight_Param.RGB_Light_Color_Sequence	=	5;
				}
				LC_RGBLight_Reserve_Mode();
			}
			//	set Mic Sensitiviyt
			else if((LC_App_Set_Param.app_write_data[1] == 0x04) && (LC_App_Set_Param.app_write_data[2] == 0x06))
			{
				LC_RGBLight_Param.RGB_Mic_Sensitivity	=	100 - LC_App_Set_Param.app_write_data[3];
			}
			if(LC_RGBLight_Param.RGB_Light_State == State_On)
			{
				//	RGBLight Static Color Plate
				if((LC_App_Set_Param.app_write_data[1] == 0x07) && (LC_App_Set_Param.app_write_data[2] == 0x05))
				{
				#if(LC_RGBLight_Mic_Enable == 1)
					LC_Mic_Sample_Stop();
				#endif	//	LC_RGBLight_Mic_Enable
					LC_RGBLight_Param.RGB_Light_Mode_Auto	=	State_Off;
					LC_RGBLight_Param.RGB_Light_Mode		=	RGB_Plate_Mode;
					LC_RGBLight_Param.RGB_rValue_New		=	LC_App_Set_Param.app_write_data[4];
					LC_RGBLight_Param.RGB_gValue_New		=	LC_App_Set_Param.app_write_data[5];
					LC_RGBLight_Param.RGB_bValue_New		=	LC_App_Set_Param.app_write_data[6];
				#if(LC_RGBLight_Module == RGBWLight)
					LC_RGBLight_Param.RGB_wValue_New		=	0;
					LC_RGBLight_Param.RGB_wValue			=	0;
				#endif
					LC_RGBLight_Param.RGB_rValue			=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
					LC_RGBLight_Param.RGB_gValue			=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
					LC_RGBLight_Param.RGB_bValue			=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
					LC_PWMSetRGBValue();
					LC_RGBLight_Reserve_Mode();
				}
				//	RGBLight Lightness Level Slider
				else if((LC_App_Set_Param.app_write_data[1] == 0x04) && (LC_App_Set_Param.app_write_data[2] == 0x01))
				{
					if((LC_RGBLight_Param.RGB_Light_Mode	==	RGB_Plate_Mode) || \
						((LC_RGBLight_Param.RGB_Light_Mode	>= RGB_Static_Red) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Static_White)))
					{
					#if(LC_RGBLight_Mic_Enable == 1)
						LC_Mic_Sample_Stop();
					#endif	//	LC_RGBLight_Mic_Enable
						LC_RGBLight_Param.RGB_Light_Level	=	LC_App_Set_Param.app_write_data[3];
						LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
						LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
						LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
					#if(LC_RGBLight_Module == RGBWLight)
						LC_RGBLight_Param.RGB_wValue		=	LC_RGBLight_Param.RGB_wValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
					#endif
						LC_PWMSetRGBValue();
						LC_RGBLight_Reserve_Mode();
					}
				}
				//	RGBLight Cool Wihte Plate
				else if((LC_App_Set_Param.app_write_data[1] == 0x05) && (LC_App_Set_Param.app_write_data[2] == 0x05))
				{
				#if(LC_RGBLight_Mic_Enable == 1)
					LC_Mic_Sample_Stop();
				#endif	//	LC_RGBLight_Mic_Enable
					LC_RGBLight_Param.RGB_Light_Mode_Auto	=	State_Off;
					LC_RGBLight_Param.RGB_Light_Mode		=	RGB_Plate_Mode;
					LC_RGBLight_Param.RGB_rValue			=	RGB_WHITE_MAX*LC_App_Set_Param.app_write_data[4]/RGB_LEVEL_PECENT;
					LC_RGBLight_Param.RGB_gValue			=	RGB_WHITE_MAX*LC_App_Set_Param.app_write_data[4]/RGB_LEVEL_PECENT;
					LC_RGBLight_Param.RGB_bValue			=	RGB_WHITE_MAX*LC_App_Set_Param.app_write_data[4]/RGB_LEVEL_PECENT;
				#if(LC_RGBLight_Module ==	RGBWLight)
					LC_RGBLight_Param.RGB_wValue			=	0;
					LC_RGBLight_Param.RGB_wValue_New		=	0;
				#endif
					LC_RGBLight_Param.RGB_rValue_New		=	LC_RGBLight_Param.RGB_rValue;
					LC_RGBLight_Param.RGB_gValue_New		=	LC_RGBLight_Param.RGB_gValue;
					LC_RGBLight_Param.RGB_bValue_New		=	LC_RGBLight_Param.RGB_bValue;
					LC_PWMSetRGBValue();
					LC_RGBLight_Reserve_Mode();
				}
				//	RGBLight Warm Withe Plate
				else if((LC_App_Set_Param.app_write_data[1] == 0x06) && (LC_App_Set_Param.app_write_data[2] == 0x05))
				{
				#if(LC_RGBLight_Mic_Enable == 1)
					LC_Mic_Sample_Stop();
				#endif	//	LC_RGBLight_Mic_Enable
					LC_RGBLight_Param.RGB_Light_Mode_Auto	=	State_Off;
					LC_RGBLight_Param.RGB_Light_Mode		=	RGB_Plate_Mode;
				#if(LC_RGBLight_Module ==	RGBLight)
					LC_RGBLight_Param.RGB_rValue			=	RGB_WHITE_MAX;
					LC_RGBLight_Param.RGB_gValue			=	RGB_WHITE_MAX;
					LC_RGBLight_Param.RGB_bValue			=	RGB_WHITE_MAX*LC_App_Set_Param.app_write_data[5]/RGB_LEVEL_PECENT;
				#elif(LC_RGBLight_Module == RGBWLight)
					LC_RGBLight_Param.RGB_rValue			=	0;
					LC_RGBLight_Param.RGB_gValue			=	0;
					LC_RGBLight_Param.RGB_bValue			=	0;
					LC_RGBLight_Param.RGB_wValue			=	RGB_WHITE_MAX*LC_App_Set_Param.app_write_data[4]/RGB_LEVEL_PECENT;
					LC_RGBLight_Param.RGB_wValue_New		=	LC_RGBLight_Param.RGB_wValue;
				#endif
					LC_RGBLight_Param.RGB_rValue_New		=	LC_RGBLight_Param.RGB_rValue;
					LC_RGBLight_Param.RGB_gValue_New		=	LC_RGBLight_Param.RGB_gValue;
					LC_RGBLight_Param.RGB_bValue_New		=	LC_RGBLight_Param.RGB_bValue;
					LC_PWMSetRGBValue();
					LC_RGBLight_Reserve_Mode();
				}
				//	RGBLight Mode Choose
				else if((LC_App_Set_Param.app_write_data[1] == 0x05) && (LC_App_Set_Param.app_write_data[2] == 0x03))
				{
					if(LC_App_Set_Param.app_write_data[4] == 0x03)
					{
					#if(LC_RGBLight_Mic_Enable == 1)
						LC_Mic_Sample_Stop();
					#endif	//	LC_RGBLight_Mic_Enable
						if((LC_App_Set_Param.app_write_data[3] >= APP_STATIC_RED) && (LC_App_Set_Param.app_write_data[3] <= APP_STATIC_WHITE))
						{
							LC_RGBLight_Param.RGB_Light_Mode	=	LC_App_Set_Param.app_write_data[3] - 0x20;
							LC_RGBLight_Mode_Static_OneColor(LC_RGBLight_Param.RGB_Light_Mode);
						}
						else if(LC_App_Set_Param.app_write_data[3] == APP_JUMP_THREE)
						{
							LC_RGBLight_Param.RGB_Light_Mode	=	RGB_Jump_ThreeColors;
						}
						else if(LC_App_Set_Param.app_write_data[3] == APP_JUMP_SEVEN)
						{
							LC_RGBLight_Param.RGB_Light_Mode	=	RGB_Jump_SevenColors;
						}
						else if((LC_App_Set_Param.app_write_data[3] >= APP_FADE_THREE) && (LC_App_Set_Param.app_write_data[3] <= APP_FADE_G_B))
						{
							LC_RGBLight_Param.RGB_Light_Mode	=	LC_App_Set_Param.app_write_data[3] - 0x19;
							// LOG("fade mode 0x%x\n",LC_RGBLight_Param.RGB_Light_Mode);
						}
						else if((LC_App_Set_Param.app_write_data[3] >= APP_FLASH_SEVEN) && (LC_App_Set_Param.app_write_data[3] <= APP_FLASH_WHITE))
						{
							LC_RGBLight_Param.RGB_Light_Mode	=	LC_App_Set_Param.app_write_data[3] - 0x15;
						}
						LC_RGBLight_Modetick	=	0;
						LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
						LC_RGBLight_Param.RGB_Light_Mode_Auto		=	State_Off;
						LC_RGBLight_Param.RGB_Mode_Change_Speed		=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
						LC_RGBLight_Reserve_Mode();
					}
				#if(LC_RGBLight_Mic_Enable == 1)
					//	RGBLight Microphone Mode
					else if(LC_App_Set_Param.app_write_data[4] == 0x04){
						LC_Mic_Sample_Start();
						LC_RGBLight_Param.RGB_Light_Mode_Auto	=	State_Off;
						LC_RGBLight_Param.RGB_Light_Mode		=	LC_App_Set_Param.app_write_data[3] - 0x70;
						LC_RGBLight_Reserve_Mode();
					}
				#endif
				}
				//	RGBLight Mode Speed
				else if((LC_App_Set_Param.app_write_data[1] == 0x04) && (LC_App_Set_Param.app_write_data[2] == 0x02))
				{
					LC_RGBLight_Param.RGB_Speed_Reserved	=	LC_App_Set_Param.app_write_data[3];
					LC_RGBLight_Param.RGB_Mode_Change_Speed	=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
					LC_RGBLight_Reserve_Mode();
				}
			}
		}
		return(events ^ DEAL_APP_DATA_EVT);
	}

	if(events & SAVE_MODE_DATA_EVT)
	{
		uint8	EEPROM_Reserved_Mode[16]	=	{0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

		EEPROM_Reserved_Mode[1]	=	LC_RGBLight_Param.RGB_Light_Mode;
		if((LC_RGBLight_Param.RGB_Light_Mode == RGB_Plate_Mode) || ((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Static_Red) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Static_White)))
		{
		#if(LC_RGBLight_PWM_U16_Enable == 1)
			EEPROM_Reserved_Mode[2]	=	(uint8)((LC_RGBLight_Param.RGB_rValue >> 8) & 0xff);
			EEPROM_Reserved_Mode[4]	=	(uint8)((LC_RGBLight_Param.RGB_gValue >> 8) & 0xff);
			EEPROM_Reserved_Mode[6]	=	(uint8)((LC_RGBLight_Param.RGB_bValue >> 8) & 0xff);
		#if(LC_RGBLight_Module == RGBWLight)
			EEPROM_Reserved_Mode[8]	=	(uint8)((LC_RGBLight_Param.RGB_wValue >> 8) & 0xff);
		#endif	//	LC_RGBLight_Module
		#endif	//	LC_RGBLight_PWM_U16_Enable
			EEPROM_Reserved_Mode[3]	=	(uint8)(LC_RGBLight_Param.RGB_rValue & 0xff);
			EEPROM_Reserved_Mode[5]	=	(uint8)(LC_RGBLight_Param.RGB_gValue & 0xff);
			EEPROM_Reserved_Mode[7]	=	(uint8)(LC_RGBLight_Param.RGB_bValue & 0xff);
		#if(LC_RGBLight_Module == RGBWLight)
			EEPROM_Reserved_Mode[9]	=	(uint8)(LC_RGBLight_Param.RGB_wValue & 0xff);
		#endif
		}

		EEPROM_Reserved_Mode[12]	=	(uint8)((LC_RGBLight_Param.RGB_Mode_Change_Speed >> 8) & 0xff);
		EEPROM_Reserved_Mode[13]	=	(uint8)(LC_RGBLight_Param.RGB_Mode_Change_Speed  & 0xff);
		EEPROM_Reserved_Mode[14]	=	LC_RGBLight_Param.RGB_Light_Color_Sequence;

		AT24CXX_Write(EEPROM_RGB_MODE_DATA, EEPROM_Reserved_Mode, 16);
		LOG("write flash mode\n");
		return(events ^ SAVE_MODE_DATA_EVT);
	}
	
	if(events & SAVE_ALARM_DATA_EVT)
	{
		uint8	EEPROM_Reserved_Alarm[16]	=	{0xaa,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

		EEPROM_Reserved_Alarm[1]	=	LC_Dev_RTC8563_Alarm[0].Hour;
		EEPROM_Reserved_Alarm[2]	=	LC_Dev_RTC8563_Alarm[0].Minute;
		EEPROM_Reserved_Alarm[3]	=	LC_Dev_RTC8563_Alarm[0].Second;
		EEPROM_Reserved_Alarm[4]	=	LC_Dev_RTC8563_Alarm[0].Func;
		EEPROM_Reserved_Alarm[5]	=	LC_Dev_RTC8563_Alarm[0].Enable;
		EEPROM_Reserved_Alarm[6]	=	LC_Dev_RTC8563_Alarm[0].WeekDay;

		EEPROM_Reserved_Alarm[7]	=	LC_Dev_RTC8563_Alarm[1].Hour;
		EEPROM_Reserved_Alarm[8]	=	LC_Dev_RTC8563_Alarm[1].Minute;
		EEPROM_Reserved_Alarm[9]	=	LC_Dev_RTC8563_Alarm[1].Second;
		EEPROM_Reserved_Alarm[10]	=	LC_Dev_RTC8563_Alarm[1].Func;
		EEPROM_Reserved_Alarm[11]	=	LC_Dev_RTC8563_Alarm[1].Enable;
		EEPROM_Reserved_Alarm[12]	=	LC_Dev_RTC8563_Alarm[1].WeekDay;

		AT24CXX_Write(EEPROM_RGB_ALARM_DATA, EEPROM_Reserved_Alarm, 16);
		LOG("write flash alarm\n");
		return(events ^ SAVE_ALARM_DATA_EVT);
	}

    // Discard unknown events

    return 0;
}
/** @}*/

