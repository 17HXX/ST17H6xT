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
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Key.h"
#include "LC_Event_Handler.h"

#if(LC_RGBLight_Key_Enable == 1)
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint8 LC_Key_TaskID;
lc_key_struct_data LC_Key_Param = 
{
    .key_down_sys_tick = 0,
    .key_down_flag = 0,
    .key_repeated_num = 0,

};

/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/
/**
 * @brief	scanf key.
 * 
 */
static void LC_KeyScanf(void)
{
    uint8 lc_keynewvalue = 0;
    static uint8 lc_keyoldvalue = 0;

    if (gpio_read(GPIO_KEY_NO1) == 0)
    {
        lc_keynewvalue = 1;
    }

    if (lc_keynewvalue)
    {
        if ((lc_keynewvalue == 0) || (lc_keyoldvalue != lc_keynewvalue))
        {
            lc_keyoldvalue = lc_keynewvalue;
            LC_Key_Param.key_down_flag = lc_keynewvalue;
            osal_start_timerEx(LC_Key_TaskID, KEY_EVENT_LEVEL1, 10);
            osal_stop_timerEx(LC_Key_TaskID, KEY_STOPSCANF_EVT);
        }
    }
    else
    {
        if (lc_keyoldvalue != 0)
        {
            lc_keyoldvalue = 0;
            LC_Key_Param.key_down_flag = 0;
            osal_start_timerEx(LC_Key_TaskID, KEY_EVENT_LEVEL1, 10);
            osal_start_timerEx(LC_Key_TaskID, KEY_STOPSCANF_EVT, 500);
        }
    }
}
/**
 * @brief	
 * 
 */
static	int	Key_Pin_Config(void)
{

	//	key
	uint8	ret;
	gpio_dir(GPIO_KEY_NO1, IE);						//set gpio input
    gpio_pull_set(GPIO_KEY_NO1, STRONG_PULL_UP);	//pull up 10k
	ret	=	gpioin_register(GPIO_KEY_NO1, LC_Key_Pin_IntHandler_Neg, LC_Key_Pin_IntHandler_Pos);
	LOG("key init ret = %02x\n",ret);

	return	ret;
}
/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/


/*!
 *	@fn			LC_Key_Gpio_Init
 *	@brief		Initialize the key pins. 
 *	@param[in]	none.
 *	@return		none.
 */
void LC_Key_Gpio_Init(void)
{
    Key_Pin_Config();
}
/*!
 *	@fn			LC_Key_Task_Init 
 *	@brief		Initialize function for the KEY Task. 
 *	@param[in]	task_id		: 	the ID assigned by OSAL,
 *								used to send message and set timer.
 *	@retrurn	none.
 */
void LC_Key_Task_Init(uint8 task_id)
{
    LC_Key_TaskID = task_id;
	LC_Key_Gpio_Init();
	LOG("Key task init down\n");
}
/*!
 *	@fn			LC_Key_ProcessEvent
 *	@brief		KEY Task event processor.This function
 *				is called to processs all events for the task.Events
 *				include timers,messages and any other user defined events.
 *	@param[in]	task_id			:The OSAL assigned task ID.
 *	@param[in]	events			:events to process.This is a bit map and can
 *									contain more than one event.
 */
uint16 LC_Key_ProcessEvent(uint8 task_id, uint16 events)
{
	VOID task_id; // OSAL required parameter that isn't used in this function
	if (events & SYS_EVENT_MSG)
	{
		uint8 *pMsg;
		if ((pMsg = osal_msg_receive(LC_Key_TaskID)) != NULL)
		{
			LC_Common_ProcessOSALMsg((osal_event_hdr_t *)pMsg);
			// Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}
		return (events ^ SYS_EVENT_MSG);
	}
    if (events & KEY_EVENT_LEVEL1)
    {
        static uint8 LC_last_button_pressed = 0;
        static uint8 LC_last_button_numbale = 0;

        static uint32 LC_last_button_press_time = 0;
        static uint32 LC_last_button_release_time = 0;
        static uint32 LC_key_time_temp = 0;

        static uint8 Key_Long_Press_3s_Enable = 0;
        static uint8 Key_Press_Once_Enable = 0;

        LC_key_time_temp = get_systick() | 1;
        if (LC_Key_Param.key_down_flag)
        {
            if (LC_last_button_numbale)
            {
                if (clock_time_exceed_func(LC_last_button_press_time, 500))
                {
                    LC_Key_Param.key_repeated_num = 0;
                    if (!Key_Long_Press_3s_Enable)
                    {
                        Key_Long_Press_3s_Enable = 1;
                        LOG("Key_Long_Press:\n");
                        {

                        }
                    }
                }
            }
        }
        else
        {
            if (Key_Long_Press_3s_Enable)
            {
                Key_Long_Press_3s_Enable	= 0;
                Key_Press_Once_Enable		= State_Off;
                LOG("Key_Long_Release:\n");

            }
        }

        if (LC_Key_Param.key_down_flag)
        {
            if (!LC_last_button_pressed && clock_time_exceed_func(LC_last_button_release_time, 50))
            {
                LC_last_button_pressed = 1;
                LC_last_button_press_time = LC_key_time_temp;
                LC_last_button_numbale = LC_Key_Param.key_down_flag;

                if (Key_Press_Once_Enable == State_Off)
                {
                    Key_Press_Once_Enable = State_On;
                    LOG("Smoke Once\n");

                }
            }
        }
        else
        {
            if (LC_last_button_pressed && clock_time_exceed_func(LC_last_button_press_time, 20))
            {
                LC_last_button_release_time = LC_key_time_temp;
                LC_last_button_pressed = 0;
                if (Key_Press_Once_Enable == State_On)
                {
                    Key_Press_Once_Enable = State_Off;
                    LOG("Key Once Release:\n");

				}
            }
        }
        if (LC_Key_Param.key_repeated_num && LC_Key_Param.key_down_sys_tick && clock_time_exceed_func(LC_Key_Param.key_down_sys_tick, 400))
        {
            // LOG("Key total Kick num: %d\n", LC_Key_Param.key_repeated_num);
            LC_Key_Param.key_down_sys_tick = 0;
            LC_Key_Param.key_repeated_num = 0;
            LC_last_button_numbale = 0;
        }
        if (LC_last_button_numbale && !LC_Key_Param.key_down_flag && clock_time_exceed_func(LC_last_button_press_time, 20))
        {
            // LC_Key_Param.key_repeated_num++ ;
            LC_Key_Param.key_down_sys_tick = LC_key_time_temp;
            // LOG("key time num: %d, key is%d\n", LC_Key_Param.key_repeated_num, LC_last_button_numbale);
            LC_last_button_numbale = 0;
        }
        if (LC_Key_Param.key_down_flag || LC_Key_Param.key_repeated_num)
        {
            osal_start_timerEx(LC_Key_TaskID, KEY_EVENT_LEVEL1, 20);
        }
        return (events ^ KEY_EVENT_LEVEL1);
    }

    if (events & KEY_SCANF_EVT)
    {
        osal_start_timerEx(LC_Key_TaskID, KEY_SCANF_EVT, 40);
        LC_KeyScanf();
        return (events ^ KEY_SCANF_EVT);
    }

    if (events & KEY_STOPSCANF_EVT)
    {
        LOG("stop scanf \n");
        gpioin_register(GPIO_KEY_NO1, LC_Key_Pin_IntHandler_Neg, LC_Key_Pin_IntHandler_Pos);
        osal_stop_timerEx(LC_Key_TaskID, KEY_SCANF_EVT);
        return (events ^ KEY_STOPSCANF_EVT);
    }

    // Discard unknown events
    return 0;
}

#endif	//	LC_RGBLight_Key_Enable
/** @}*/
