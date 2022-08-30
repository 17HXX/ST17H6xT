/**
 *	@file		LC_RGBLight_Mode.h
 *	@author		YQ(1500861686)
 *	@date		12/19/2020
 *	@version	1.0.0
 */

/*!
 *	@defgroup	LC_RGBLight_Mode
 *	@brief
 *	@{*/

#ifndef		LC_RGBLIGHT_MODE_H_
#define		LC_RGBLIGHT_MODE_H_
/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef	__cplusplus
	extern "C"	{
#endif
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Common.h"
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
#define		RGB_Fade_Change_Step		5
#define		RGB_IR_BUFFER_DIYOFFSET		24
/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						Data structures								*/
/*------------------------------------------------------------------*/
typedef enum
{
	RGB_Turn_Off			=	0x00,
	RGB_Plate_Mode			=	0x01,
	RGB_DIY_Mode,
	
	RGB_Mic_Mode_Classical	=	0x10,
	RGB_Mic_Mode_Soft,
	RGB_Mic_Mode_Beats,
	RGB_Mic_Mode_Disco,
	
	RGB_Static_Red			=	0x60,
	RGB_Static_Blue,
	RGB_Static_Green,
	RGB_Static_Cyan,
	RGB_Static_Yellow,
	RGB_Static_Magenta,
	RGB_Static_White,

	RGB_Fade_ThreeColors	=	0x70,	/*red-green-blue							*/
	RGB_Fade_SevenColors,				/*red-green-blue-yellow-cyan-magenta-white	*/
	RGB_Fade_Red,
	RGB_Fade_Green,
	RGB_Fade_Blue,
	RGB_Fade_Yellow,					/*red	&	green							*/
	RGB_Fade_Cyan,						/*green	&	blue							*/
	RGB_Fade_Magenta,					/*red	&	blue							*/
	RGB_Fade_White,						/*red	&	green	&	blue				*/
	RGB_Fade_Red_Green,
	RGB_Fade_Red_Blue,
	RGB_Fade_Green_Blue,

	RGB_Flash_SevenColors	=	0x80,	/*red-green-blue-yellow-cyan-magenta-white	*/
//	RGB_Flash_ThreeColors,
	RGB_Flash_Red,
	RGB_Flash_Green,
	RGB_Flash_Blue,
	RGB_Flash_Yellow,
	RGB_Flash_Cyan,
	RGB_Flash_Magenta,
	RGB_Flash_White,

	RGB_Jump_ThreeColors	=	0x90,	/*red-green-blue							*/
	RGB_Jump_SevenColors,				/*red-green-blue-yellow-cyan-magenta-white	*/
//	RGB_Jump_AllStaticColors,

	RGB_Smooth				=	0xa0,

	
}lc_rgblight_all_mode;

/*------------------------------------------------------------------*/
/*						external variables							*/
/*------------------------------------------------------------------*/
extern	uint16	LC_RGBLight_Modetick;
extern	const	uint8	LC_RGBLight_Mode_Buffer[24];
/*------------------------------------------------------------------*/
/*						User function prototypes					*/
/*------------------------------------------------------------------*/
void	LC_RGBLight_Dynamic_Basic_Timer		(void					);
void	LC_RGBLight_Mode_Static_OneColor	(uint8 mode				);
void	LC_RGBLight_Mode_Jump				(uint8 jump_color_num,	const uint8 *rgb_data,	uint16 jump_speed	);
void	LC_RGBLight_Mode_Flash				(uint8 flash_color_num,	const uint8 *rgb_data,	uint16 flash_speed	);
void	LC_RGBLight_Mode_Fade				(uint8 fade_color_num,	const uint8 *rgb_data,	uint16 fade_speed	);
void	LC_RGBLight_Mode_Smooth				(uint16 smooth_speed												);
void	LC_RGBLight_Mode_Static_Fade		(uint8 static_fade_mode,uint16 static_fade_speed					);
uint16	LC_RGBLight_Mode_Speed				(uint16 original_speed	);
void	LC_RGBLight_Turn_Onoff				(uint8 onoff			);
void	LC_RGBLight_Dynamic_Mode_Process	(void					);

#ifdef	__cplusplus
	}
#endif

#endif	/**	LC_RGBLight_Mode.h **/
/**	@}*/
