/**
 *	@file		LC_RGBLight_Mode.c
 *	@author		YQ(1500861686)
 *	@date		12/19/2020
 *	@version	1.0.0
 */

/*!
 *	@defgroup	LC_RGBLight_Mode
 *	@brief
 *	@{*/
/*------------------------------------------------------------------*/
/* 					 head files include 						 	*/
/*------------------------------------------------------------------*/
#include "LC_RGBLight_Mode.h"
#include "LC_UI_Led_Buzzer.h"
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint16	LC_RGBLight_Modetick	=	0;
const	uint8	LC_RGBLight_Mode_Buffer[24]	=	
{
	255,   0,   0,		0, 255,   0,		  0,   0, 255,	//	red,		green,	blue,
	255, 255,   0,		0, 255, 255,		255,   0, 255,	//	yellow,	cyan,	magenta,
	RGB_WHITE_MAX, RGB_WHITE_MAX, RGB_WHITE_MAX,			//	white,
};
/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/*!
 *	@fn			LC_RGBLight_Dynamic_Basic_Timer
 *	@brief		basic timer tick of RGBLight mode changing. 
 *	@param[in]	none.
 *	@return		none.
 */
void	LC_RGBLight_Dynamic_Basic_Timer(void)
{
	static	uint8	time_cnt	=	0;
	static	uint8	write_reg_timer_cnt	=	0;

	time_cnt++;
	if(time_cnt > 19)
	{
		time_cnt	=	0;
		LC_RGBLight_Modetick++;
		write_reg_timer_cnt++;
		if(write_reg_timer_cnt > 2)
		{
			write_reg_timer_cnt	=	0;
			if(LC_RGBLight_Param.RGB_Light_Mode > RGB_Static_White)
			{
				LC_RGBLight_Dynamic_Mode_Process();
			}
		}
	}
}
/*------------------------------------------------------------------*/
/* 					 							 					*/
/*------------------------------------------------------------------*/
void	LC_RGBLight_Static_Red(void)
{
	LC_RGBLight_Param.RGB_rValue_New	=	RGB_COLOR_MAX;
	LC_RGBLight_Param.RGB_gValue_New	=	0;
	LC_RGBLight_Param.RGB_bValue_New	=	0;
#if(LC_RGBLight_Module ==	RGBWLight)
	LC_RGBLight_Param.RGB_wValue_New	=	0;
	LC_RGBLight_Param.RGB_wValue		=	0;
#endif
	LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
}
void	LC_RGBLight_Static_Green(void)
{
	LC_RGBLight_Param.RGB_rValue_New	=	0;
	LC_RGBLight_Param.RGB_gValue_New	=	RGB_COLOR_MAX;
	LC_RGBLight_Param.RGB_bValue_New	=	0;
#if(LC_RGBLight_Module ==	RGBWLight)
	LC_RGBLight_Param.RGB_wValue_New	=	0;
	LC_RGBLight_Param.RGB_wValue		=	0;
#endif
	LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
}
void	LC_RGBLight_Static_Blue(void)
{
	LC_RGBLight_Param.RGB_rValue_New	=	0;
	LC_RGBLight_Param.RGB_gValue_New	=	0;
	LC_RGBLight_Param.RGB_bValue_New	=	RGB_COLOR_MAX;
#if(LC_RGBLight_Module ==	RGBWLight)
	LC_RGBLight_Param.RGB_wValue_New	=	0;
	LC_RGBLight_Param.RGB_wValue		=	0;
#endif
	LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
}
void	LC_RGBLight_Static_Yellow(void)
{
	LC_RGBLight_Param.RGB_rValue_New	=	RGB_COLOR_MAX;
	LC_RGBLight_Param.RGB_gValue_New	=	RGB_COLOR_MAX;
	LC_RGBLight_Param.RGB_bValue_New	=	0;
#if(LC_RGBLight_Module ==	RGBWLight)
	LC_RGBLight_Param.RGB_wValue_New	=	0;
	LC_RGBLight_Param.RGB_wValue		=	0;
#endif
	LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
}
void	LC_RGBLight_Static_Cyan(void)
{
	LC_RGBLight_Param.RGB_rValue_New	=	0;
	LC_RGBLight_Param.RGB_gValue_New	=	RGB_COLOR_MAX;
	LC_RGBLight_Param.RGB_bValue_New	=	RGB_COLOR_MAX;
#if(LC_RGBLight_Module ==	RGBWLight)
	LC_RGBLight_Param.RGB_wValue_New	=	0;
	LC_RGBLight_Param.RGB_wValue		=	0;
#endif
	LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
}
void	LC_RGBLight_Static_Magenta(void)
{
	LC_RGBLight_Param.RGB_rValue_New	=	RGB_COLOR_MAX;
	LC_RGBLight_Param.RGB_gValue_New	=	0;
	LC_RGBLight_Param.RGB_bValue_New	=	RGB_COLOR_MAX;
#if(LC_RGBLight_Module ==	RGBWLight)
	LC_RGBLight_Param.RGB_wValue_New	=	0;
	LC_RGBLight_Param.RGB_wValue		=	0;
#endif
	LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
}
void	LC_RGBLight_Static_White(void)
{
	LC_RGBLight_Param.RGB_rValue_New	=	RGB_WHITE_MAX;
	LC_RGBLight_Param.RGB_gValue_New	=	RGB_WHITE_MAX;
	LC_RGBLight_Param.RGB_bValue_New	=	RGB_WHITE_MAX;
#if(LC_RGBLight_Module ==	RGBWLight)
	LC_RGBLight_Param.RGB_wValue_New	=	0;
	LC_RGBLight_Param.RGB_wValue		=	0;
#endif
	LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
}
void	LC_RGBLight_Mode_Static_OneColor(uint8 mode)
{
	switch(mode)
	{
		case RGB_Static_Red:
			LC_RGBLight_Static_Red();
			break;
		case RGB_Static_Green:
			LC_RGBLight_Static_Green();
			break;
		case RGB_Static_Blue:
			LC_RGBLight_Static_Blue();
			break;
		case RGB_Static_Cyan:
			LC_RGBLight_Static_Cyan();
			break;
		case RGB_Static_Yellow:
			LC_RGBLight_Static_Yellow();
			break;
		case RGB_Static_Magenta:
			LC_RGBLight_Static_Magenta();
			break;
		case RGB_Static_White:
			LC_RGBLight_Static_White();
			break;
		default:
			break;
	}
	LC_PWMSetRGBValue();
}
/*------------------------------------------------------------------*/
/* 					 							 					*/
/*------------------------------------------------------------------*/
/*!
 *	@fn			LC_RGBLight_Mode_Jump
 *	@brief		Strobe(jump) mode. 
 *	@param[in]	jump_color_num		:color number.
 *	@param[in]	rgb_data			:pointer of color.
 *	@param[in]	jump_speed			:speed of mode changing.
 *	@return		none.
 */
void	LC_RGBLight_Mode_Jump(uint8 jump_color_num, const uint8 *rgb_data, uint16 jump_speed)
{
	if(LC_RGBLight_Modetick > jump_speed)
	{
		LC_RGBLight_Modetick	=	0;
		LC_RGBLight_Param.RGB_rValue	=	(*(rgb_data + LC_RGBLight_Param.RGB_Mode_Change_Color_Num*3));
		LC_RGBLight_Param.RGB_gValue	=	(*(rgb_data + LC_RGBLight_Param.RGB_Mode_Change_Color_Num*3 + 1));
		LC_RGBLight_Param.RGB_bValue	=	(*(rgb_data + LC_RGBLight_Param.RGB_Mode_Change_Color_Num*3 + 2));
	#if(LC_RGBLight_Module ==	RGBWLight)
		LC_RGBLight_Param.RGB_wValue	=	0;
	#endif
		LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
		if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num >= jump_color_num)
		{
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
			if(LC_RGBLight_Param.RGB_Light_Mode_Auto == State_On)
			{
				LC_RGBLight_Modetick	=	0;
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
				LC_RGBLight_Param.RGB_Light_Mode			=	RGB_Fade_SevenColors;
				LC_RGBLight_Param.RGB_Mode_Change_Speed		=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
			}
		}
		LC_PWMSetRGBValue();
	}
}
/*!
 *	@fn			LC_RGBLight_Mode_Flash
 *	@brief		flash mode. 
 *	@param[in]	flash_color_num		:color number.
 *	@param[in]	rgb_data			:pointer of color.
 *	@param[in]	flash_speed			:speed of mode changing.
 *	@return		none.
 */
void	LC_RGBLight_Mode_Flash(uint8 flash_color_num, const uint8 *rgb_data, uint16 flash_speed)
{
	if(LC_RGBLight_Modetick > flash_speed)
	{
		LC_RGBLight_Modetick	=	0;
		if((LC_RGBLight_Param.RGB_Mode_Change_Color_Num % 2) == 0)
		{
			LC_RGBLight_Param.RGB_rValue	=	(*(rgb_data + (LC_RGBLight_Param.RGB_Mode_Change_Color_Num/2)*3));
			LC_RGBLight_Param.RGB_gValue	=	(*(rgb_data + (LC_RGBLight_Param.RGB_Mode_Change_Color_Num/2)*3 + 1));
			LC_RGBLight_Param.RGB_bValue	=	(*(rgb_data + (LC_RGBLight_Param.RGB_Mode_Change_Color_Num/2)*3 + 2));
		}
		else
		{
			LC_RGBLight_Param.RGB_rValue	=	0;
			LC_RGBLight_Param.RGB_gValue	=	0;
			LC_RGBLight_Param.RGB_bValue	=	0;
		}
	#if(LC_RGBLight_Module ==	RGBWLight)
		LC_RGBLight_Param.RGB_wValue	=	0;
	#endif
		LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
		if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num >= flash_color_num*2)
		{
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
			if(LC_RGBLight_Param.RGB_Light_Mode_Auto == State_On)
			{
				LC_RGBLight_Param.RGB_Mode_Flash_Time_Num++;
				if(LC_RGBLight_Param.RGB_Mode_Flash_Time_Num > 3)
				{
					LC_RGBLight_Param.RGB_Mode_Flash_Time_Num	=	0;
					LC_RGBLight_Modetick	=	0;
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num =	0;
					LC_RGBLight_Param.RGB_Light_Mode		=	RGB_Jump_SevenColors;
					LC_RGBLight_Param.RGB_Mode_Change_Speed =	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
				}
			}
		}
		LC_PWMSetRGBValue();
	}
}
/*!
 *	@fn			LC_RGBLight_Mode_Fade
 *	@brief		flash mode. 
 *	@param[in]	flash_color_num		:color number.
 *	@param[in]	rgb_data			:pointer of color.
 *	@param[in]	flash_speed			:speed of mode changing.
 *	@return		none.
 */
void	LC_RGBLight_Mode_Fade(uint8 fade_color_num, const uint8 *rgb_data, uint16 fade_speed)
{
	static	uint8	LC_RGBLight_Pecent_Cnt	=	0;
	if((LC_RGBLight_Param.RGB_Mode_Change_Color_Num) == 0)
	{
		LC_RGBLight_Pecent_Cnt	=	0;
		LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
	}
	else if((LC_RGBLight_Param.RGB_Mode_Change_Color_Num % 2) == 1)
	{
		if(LC_RGBLight_Modetick > fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Pecent_Cnt < 99)
			{
				LC_RGBLight_Pecent_Cnt += 2;
			}
			else
			{
                LC_RGBLight_Pecent_Cnt  =   100;
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if((LC_RGBLight_Param.RGB_Mode_Change_Color_Num % 2) == 0)
	{
		if(LC_RGBLight_Modetick > fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Pecent_Cnt > 0)
			{
				LC_RGBLight_Pecent_Cnt -= 2;
			}
			else
			{
                LC_RGBLight_Pecent_Cnt  =   0;
                LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
                if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num >= fade_color_num*2)
				{
                    LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
                }
			}
		}
	}
	LC_RGBLight_Param.RGB_rValue	=	(*(rgb_data + ((LC_RGBLight_Param.RGB_Mode_Change_Color_Num-1)/2)*3 + 0))*LC_RGBLight_Pecent_Cnt/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_gValue	=	(*(rgb_data + ((LC_RGBLight_Param.RGB_Mode_Change_Color_Num-1)/2)*3 + 1))*LC_RGBLight_Pecent_Cnt/RGB_LEVEL_PECENT;
	LC_RGBLight_Param.RGB_bValue	=	(*(rgb_data + ((LC_RGBLight_Param.RGB_Mode_Change_Color_Num-1)/2)*3 + 2))*LC_RGBLight_Pecent_Cnt/RGB_LEVEL_PECENT;
#if(LC_RGBLight_Module ==	RGBWLight)
	LC_RGBLight_Param.RGB_wValue	=	0;
#endif
	LC_PWMSetRGBValue();
}
/*!
 *	@fn			LC_RGBLight_Mode_Smooth
 *	@brief		Smooth mode. 
 *	@param[in]	smooth_speed			:speed of mode changing.
 *	@return		none.
 */
void	LC_RGBLight_Mode_Smooth(uint16 smooth_speed)
{
	if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num	==	0)
	{
		LC_RGBLight_Param.RGB_rValue	=	0;
		LC_RGBLight_Param.RGB_gValue	=	0;
		LC_RGBLight_Param.RGB_bValue	=	0;
	#if(LC_RGBLight_Module ==	RGBWLight)
		LC_RGBLight_Param.RGB_wValue	=	0;
	#endif
		LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 1)
	{
		if(LC_RGBLight_Modetick > smooth_speed){
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue < RGB_PWM_MAX)
			{		
				LC_RGBLight_Param.RGB_rValue	+=	RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 2)
	{
		if(LC_RGBLight_Modetick > smooth_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_gValue < RGB_PWM_MAX)
			{		
				LC_RGBLight_Param.RGB_gValue	+=	RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 3)
	{
		if(LC_RGBLight_Modetick > smooth_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue	> 0)
			{
				LC_RGBLight_Param.RGB_rValue	-=	RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 4)
	{
		if(LC_RGBLight_Modetick > smooth_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_bValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_bValue	+=	RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 5)
	{
		if(LC_RGBLight_Modetick > smooth_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_gValue	> 0)
			{
				LC_RGBLight_Param.RGB_gValue	-=	RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 6)
	{
		if(LC_RGBLight_Modetick > smooth_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_rValue	+=	RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 7)
	{
		if(LC_RGBLight_Modetick > smooth_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_gValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_gValue	+=	RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 8)
	{
		if(LC_RGBLight_Modetick > smooth_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_gValue	> 0)
			{
				LC_RGBLight_Param.RGB_gValue	-=	RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_bValue	-=	RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	1;
			}
		}
	}
	LC_PWMSetRGBValue();
}

/*!
 *	@fn			LC_RGBLight_Mode_Static_Fade
 *	@brief		fade mode. 
 *	@param[in]	static_fade_mode	:fade mode for one&two&three color(s).
 *	@param[in]	static_fade_speed	:speed of mode changing.
 *	@return		none.
 */
void	LC_RGBLight_Mode_Static_Fade(uint8 static_fade_mode,uint16 static_fade_speed)
{
	if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 0)
	{
		LC_RGBLight_Param.RGB_rValue	=	0;
		LC_RGBLight_Param.RGB_gValue	=	0;
		LC_RGBLight_Param.RGB_bValue	=	0;
	#if(LC_RGBLight_Module ==	RGBWLight)
		LC_RGBLight_Param.RGB_wValue	=	0;
	#endif

		if((static_fade_mode == RGB_Fade_Green) || (static_fade_mode == RGB_Fade_Green_Blue))
		{
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	3;
		}
		else if(static_fade_mode == RGB_Fade_Blue)
		{
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	5;
		}
		else if(static_fade_mode == RGB_Fade_Yellow)
		{
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	7;
		}
		else if(static_fade_mode == RGB_Fade_Cyan)
		{
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	9;
		}
		else if(static_fade_mode == RGB_Fade_Magenta)
		{
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	11;
		}
		else if(static_fade_mode == RGB_Fade_White)
		{
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	13;
		}
		else
		{
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 1)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_rValue+=RGB_Fade_Change_Step;
//				LOG("red ++%d\n",LC_RGBLight_Param.RGB_rValue);
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 2)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue > 0)
			{
				LC_RGBLight_Param.RGB_rValue-=RGB_Fade_Change_Step;
//				LOG("red --%d\n",LC_RGBLight_Param.RGB_rValue);
			}
			else
			{
				if(static_fade_mode == RGB_Fade_Red)
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
				}
				else if(static_fade_mode == RGB_Fade_Red_Blue)
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	5;
				}
				else
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
				}
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 3)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_gValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_gValue+=RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 4)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_gValue > 0)
			{
				LC_RGBLight_Param.RGB_gValue-=RGB_Fade_Change_Step;
			}
			else
			{
				if((static_fade_mode == RGB_Fade_Green)||(static_fade_mode == RGB_Fade_Red_Green))
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
				}
				else
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
				}
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 5)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_bValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_bValue+=RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 6)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_bValue > 0)
			{
				LC_RGBLight_Param.RGB_bValue-=RGB_Fade_Change_Step;
			}
			else
			{
				if(	(static_fade_mode == RGB_Fade_Blue)			||	\
					(static_fade_mode == RGB_Fade_Red_Blue)		||	\
					(static_fade_mode == RGB_Fade_Green_Blue)	||	\
					(static_fade_mode == RGB_Fade_ThreeColors))
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
				}
				else
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
				}
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 7)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_rValue+=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_gValue+=RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 8)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue > 0)
			{
				LC_RGBLight_Param.RGB_rValue-=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_gValue-=RGB_Fade_Change_Step;
			}
			else
			{
				if(static_fade_mode == RGB_Fade_Yellow)
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
				}
				else
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
				}
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 9)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_gValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_gValue+=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_bValue+=RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 10)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_gValue > 0)
			{
				LC_RGBLight_Param.RGB_gValue-=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_bValue-=RGB_Fade_Change_Step;
			}
			else
			{
				if(static_fade_mode == RGB_Fade_Cyan)
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
				}
				else
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
				}
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 11)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_rValue+=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_bValue+=RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 12)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue > 0)
			{
				LC_RGBLight_Param.RGB_rValue-=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_bValue-=RGB_Fade_Change_Step;
			}
			else
			{
				if(static_fade_mode == RGB_Fade_Magenta)
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
				}
				else
				{
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
				}
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 13)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue < RGB_PWM_MAX)
			{
				LC_RGBLight_Param.RGB_rValue+=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_gValue+=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_bValue+=RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num++;
			}
		}
	}
	else if(LC_RGBLight_Param.RGB_Mode_Change_Color_Num == 14)
	{
		if(LC_RGBLight_Modetick > static_fade_speed)
		{
			LC_RGBLight_Modetick	=	0;
			if(LC_RGBLight_Param.RGB_rValue > 0)
			{
				LC_RGBLight_Param.RGB_rValue-=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_gValue-=RGB_Fade_Change_Step;
				LC_RGBLight_Param.RGB_bValue-=RGB_Fade_Change_Step;
			}
			else
			{
				LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;;
				if(LC_RGBLight_Param.RGB_Light_Mode_Auto == State_On)
				{
					LC_RGBLight_Modetick	=	0;
					LC_RGBLight_Param.RGB_Mode_Change_Color_Num =	0;
					LC_RGBLight_Param.RGB_Mode_Flash_Time_Num	=	0;
					LC_RGBLight_Param.RGB_Light_Mode			=	RGB_Flash_White;
					LC_RGBLight_Param.RGB_Mode_Change_Speed 	=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
				}
			}
		}
	}
	LC_PWMSetRGBValue();
}

/*!
 *	@fn			LC_RGBLight_Mode_Speed
 *	@brief		speed of mode changing. 
 *	@param[in]	original_speed		:speed App setted.
 *	@return		actual speed		:LC_RGBLight_Param.RGB_Mode_Change_Speed.
 */
uint16	LC_RGBLight_Mode_Speed(uint16 original_speed)
{
	uint16	speed = 0;
	
	if((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Fade_ThreeColors) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Fade_Green_Blue))
	{
		speed	=	(30 - original_speed/5);
	}
	else if((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Flash_SevenColors) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Jump_SevenColors))
	{
		speed	=	(250 - original_speed*2);
	}
	else if(LC_RGBLight_Param.RGB_Light_Mode == RGB_Smooth)
	{
		speed	=	(25 - original_speed/5);
	}
	return	speed;
}
/*!
 *	@fn			LC_RGBLight_Turn_Onoff
 *	@brief		turn on & off RGBLight, restore mode and speed. 
 *	@param[in]	onoff			:.
 *	@return		none.
 */
void	LC_RGBLight_Turn_Onoff(uint8 onoff)
{
	if(onoff)
	{
		LC_RGBLight_Param.RGB_Light_State	=	State_On;
		LC_RGBLight_Param.RGB_Light_Mode	=	LC_RGBLight_Param.RGB_Light_Mode_Reserved;
		if(LC_RGBLight_Param.RGB_Light_Mode == RGB_Plate_Mode)
		{
			LC_RGBLight_Param.RGB_Light_Mode	=	RGB_Plate_Mode;
			LC_RGBLight_Param.RGB_rValue		=	LC_RGBLight_Param.RGB_rValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
			LC_RGBLight_Param.RGB_gValue		=	LC_RGBLight_Param.RGB_gValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
			LC_RGBLight_Param.RGB_bValue		=	LC_RGBLight_Param.RGB_bValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
		#if(LC_RGBLight_Module ==	RGBWLight)
			LC_RGBLight_Param.RGB_wValue		=	LC_RGBLight_Param.RGB_wValue_New*LC_RGBLight_Param.RGB_Light_Level/RGB_LEVEL_PECENT;
		#endif
			LC_PWMSetRGBValue();
		}
	#if(LC_RGBLight_Mic_Enable == 1)	
		else if((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Mic_Mode_Classical) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Mic_Mode_Disco))
		{
			osal_start_timerEx(LC_Mic_Sample_TaskID, MIC_EVENT_LEVEL1, 200);	//	LC_Mic_Sample_Start
		}
	#endif
		else if((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Static_Red) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Static_White))
		{
			LC_RGBLight_Mode_Static_OneColor(LC_RGBLight_Param.RGB_Light_Mode);
		}
		else if((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Fade_ThreeColors) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Smooth))
		{
			LC_RGBLight_Modetick				=	0;
			LC_RGBLight_Param.RGB_Mode_Change_Color_Num	=	0;
			LC_RGBLight_Param.RGB_Mode_Change_Speed 	=	LC_RGBLight_Mode_Speed(LC_RGBLight_Param.RGB_Speed_Reserved);
		}
		LOG("turn on led\n");
	}
	else
	{
		LC_RGBLight_Param.RGB_Light_Mode_Reserved	=	LC_RGBLight_Param.RGB_Light_Mode;
		LC_RGBLight_Param.RGB_Light_State	=	State_Off;
		LC_RGBLight_Param.RGB_rValue		=	0;
		LC_RGBLight_Param.RGB_gValue		=	0;
		LC_RGBLight_Param.RGB_bValue		=	0;
	#if(LC_RGBLight_Module ==	RGBWLight)
		LC_RGBLight_Param.RGB_wValue		=	0;
	#endif	//	LC_RGBLight_Module
	#if(LC_RGBLight_Mic_Enable == 1)
		LC_Mic_Sample_Stop();
	#endif	//	LC_RGBLight_Mic_Enable
		LC_RGBLight_Param.RGB_Light_Mode	=	RGB_Turn_Off;
		LC_PWMSetRGBValue();
		LOG("turn off led\n");
	}
}
/*!
 *	@fn			LC_RGBLight_Dynamic_Mode_Process
 *	@brief		Mode changing of RGBLight:	strobe(jump),flash,fade,smooth. 
 *	@param[in]	none.
 *	@return		none.
 */
void	LC_RGBLight_Dynamic_Mode_Process(void)
{
	if(LC_RGBLight_Param.RGB_Light_Mode == RGB_Jump_ThreeColors)
	{
		LC_RGBLight_Mode_Jump(3, LC_RGBLight_Mode_Buffer, LC_RGBLight_Param.RGB_Mode_Change_Speed);
	}
	else if(LC_RGBLight_Param.RGB_Light_Mode == RGB_Jump_SevenColors)
	{
		LC_RGBLight_Mode_Jump(7, LC_RGBLight_Mode_Buffer, LC_RGBLight_Param.RGB_Mode_Change_Speed);
	}
	else if((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Fade_ThreeColors) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Fade_Green_Blue))
	{
		LC_RGBLight_Mode_Static_Fade(LC_RGBLight_Param.RGB_Light_Mode,LC_RGBLight_Param.RGB_Mode_Change_Speed);
	}
	else if(LC_RGBLight_Param.RGB_Light_Mode == RGB_Flash_SevenColors)
	{
		LC_RGBLight_Mode_Flash(7, LC_RGBLight_Mode_Buffer, LC_RGBLight_Param.RGB_Mode_Change_Speed);
	}
	else if((LC_RGBLight_Param.RGB_Light_Mode >= RGB_Flash_Red) && (LC_RGBLight_Param.RGB_Light_Mode <= RGB_Flash_White))
	{
		LC_RGBLight_Mode_Flash(1, LC_RGBLight_Mode_Buffer + 3*(LC_RGBLight_Param.RGB_Light_Mode - RGB_Flash_Red), LC_RGBLight_Param.RGB_Mode_Change_Speed);
	}
	else if(LC_RGBLight_Param.RGB_Light_Mode == RGB_Smooth)
	{
		LC_RGBLight_Mode_Smooth(LC_RGBLight_Param.RGB_Mode_Change_Speed);
	}
}

/** @}*/
 
