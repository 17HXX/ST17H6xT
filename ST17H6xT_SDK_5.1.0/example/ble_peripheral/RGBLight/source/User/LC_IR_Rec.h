/**
 *	@file		LC_IR_Rec.h
 *	@author		YQ(1500861686)
 *	@date		12/23/2020
 *	@version	1.0.0
 */

/*!
 *	@defgroup	LC_IR_Rec
 *	@brief
 *	@{*/

#ifndef		LC_IR_REC_H_
#define		LC_IR_REC_H_
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
//	IR Keyboard User Number Type
#define		IR_KeyBoard_Type_24Keys		0x10ff
/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/
#define		IR_REC_EVENT_LEVEL1		0x0001
#define		IR_REC_EVENT_LEVEL2		0x0002
/*------------------------------------------------------------------*/
/*						Data structures								*/
/*------------------------------------------------------------------*/
typedef	enum{
	IRKey_Light_Level_Up	=	0,		IRKey_Up		=	IRKey_Light_Level_Up,
	IRKey_Light_Level_Down	=	1,		IRKey_Down		=	IRKey_Light_Level_Down,
	IRKey_Light_Off			=	2,		IRKey_Off		=	IRKey_Light_Off,
	IRKey_Light_On			=	3,		IRKey_On		=	IRKey_Light_On,
	IRKey_Light_Red			=	4,		IRKey_Red		=	IRKey_Light_Red,
	IRKey_Light_Green		=	5,		IRKey_Green		=	IRKey_Light_Green,
	IRKey_Light_Blue		=	6,		IRKey_Blue		=	IRKey_Light_Blue,
	IRKey_Light_White		=	7,		IRKey_White		=	IRKey_Light_White,
	IRKey_Col1_Row3			=	8,		IRKey_Orange	=	IRKey_Col1_Row3,
	IRKey_Col2_Row3			=	9,		IRKey_Yellow	=	IRKey_Col2_Row3,
	IRKey_Col3_Row3			=	10,		IRKey_Cyan		=	IRKey_Col3_Row3,
	IRKey_Mode_Flash		=	11,		IRKey_Purple	=	IRKey_Mode_Flash,
	IRKey_Col1_Row4			=	12,		IRKey_Jump3		=	IRKey_Col1_Row4,
	IRKey_Col2_Row4			=	13,		IRKey_Jump7		=	IRKey_Col2_Row4,
	IRKey_Col3_Row4			=	14,		IRKey_Fade3		=	IRKey_Col3_Row4,
	IRKey_Mode_Strobe		=	15,		IRKey_Fade7		=	IRKey_Mode_Strobe,
	IRKey_Col1_Row5			=	16,		IRKey_Music1	=	IRKey_Col1_Row5,
	IRKey_Col2_Row5			=	17,		IRKey_Music2	=	IRKey_Col2_Row5,
	IRKey_Col3_Row5			=	18,		IRKey_Music3	=	IRKey_Col3_Row5,
	IRKey_Mode_Fade			=	19,		IRKey_Music4	=	IRKey_Mode_Fade,
	IRKey_Col1_Row6			=	20,
	IRKey_Col2_Row6			=	21,
	IRKey_Col3_Row6			=	22,
	IRKey_Mode_Smooth		=	23,
	
}lc_ir_keyboard_t;
/*------------------------------------------------------------------*/
/*						external variables							*/
/*------------------------------------------------------------------*/
extern	volatile	uint32	LC_IR_Analysis_100ns_Cnt;
extern	volatile	uint32	LC_IR_Analysis_KeyValue;
extern	volatile	uint32	LC_IR_Last_Posedge_Time;
extern	uint8		LC_IR_Rec_TaskID;
/*------------------------------------------------------------------*/
/*						User function prototypes					*/
/*------------------------------------------------------------------*/
void	LC_IR_Rec_Task_Init		(uint8 task_id					);
uint16	LC_IR_Rec_ProcessEvent	(uint8 task_id, uint16 events	);
#ifdef	__cplusplus
	}
#endif

#endif	/**	LC_IR_Rec.h **/
/**	@}*/
