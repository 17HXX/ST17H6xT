/**
 *	@file		LC_IIC.h
 *	@author		YQ(1500861686)
 *	@date		11/11/2020
 *	@version	1.0.0
 */

/*!
 *	@defgroup	LC_IIC
 *	@brief
 *	@{*/

#ifndef		LC_IIC_H_
#define		LC_IIC_H_
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
#define		LC_IIC_GPIO_SDA			GPIO_IIC_SDA
#define		LC_IIC_GPIO_SCL			GPIO_IIC_SCL
/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
#define		SDA_OUT()	gpio_dir(GPIO_IIC_SDA, OEN)
#define		SDA_IN()	gpio_dir(GPIO_IIC_SDA, IE)
#define		SDA_READ()	gpio_read(GPIO_IIC_SDA)

#define		SDA_OUT_H()	gpio_fast_write(GPIO_IIC_SDA, 1)
#define		SDA_OUT_L()	gpio_fast_write(GPIO_IIC_SDA, 0)

#define		SCL_OUT_H()	gpio_fast_write(GPIO_IIC_SCL, 1)
#define		SCL_OUT_L()	gpio_fast_write(GPIO_IIC_SCL, 0)

/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						Data structures								*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						external variables							*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						User function prototypes					*/
/*------------------------------------------------------------------*/
void	LC_Gpio_IIC_Init(void);
void	LC_Gpio_IIC_Deinit(void);
void	LC_IIC_Start(void);
void	LC_IIC_Stop(void);
uint8	LC_IIC_Wait_Ack(void);
void	LC_IIC_Ack(void);
void	LC_IIC_NAck(void);
void	LC_IIC_Send_Byte(uint8 data);
uint8	LC_IIC_Read_Byte(uint8 ack);


void	LC_IIC_Read_Byte_Sequence(uint8 id, uint8 addr, uint8 *p,uint8 len);
void	LC_IIC_Write_Byte_Sequence(uint8 id, uint8 addr, uint8 *p, uint8 len);
void	LC_IIC_Write_One_Byte(uint8 id, uint8 addr, uint8 data);
uint8	LC_IIC_Read_One_Byte(uint8 id, uint8 addr);
#ifdef	__cplusplus
	}
#endif

#endif	/**	LC_IIC.h **/
/**	@}*/

