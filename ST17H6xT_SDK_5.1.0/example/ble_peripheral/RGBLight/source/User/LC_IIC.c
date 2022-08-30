/**
 *	@file		LC_IIC.c
 *	@author		YQ(1500861686)
 *	@date		11/11/2020
 *	@version	1.0.0
 */

/*!
 *	@defgroup	LC_IIC
 *	@brief
 *	@{*/
 
/*------------------------------------------------------------------*/
/* 					 head files include		 						*/
/*------------------------------------------------------------------*/
#include "LC_IIC.h"

/*------------------------------------------------------------------*/
/* 					 IIC Drivers(simulated)			 				*/
/*------------------------------------------------------------------*/
static	inline	void	LC_IIC_Wait(void)
{
	;;;;;;;;;;;
}
static	inline	void	LC_IIC_Long_Wait(void)
{
	WaitUs(10);
}
/*------------------------------------------------------------------*/
/* 					 PUBLIC FUNCTIONS		 						*/
/*------------------------------------------------------------------*/

void	LC_IIC_Start(void)
{
	SDA_OUT();
	SDA_OUT_H();
	SCL_OUT_H();
	LC_IIC_Wait();
	SDA_OUT_L();	//START:when CLK is high,DATA change form high to low 
	LC_IIC_Wait();
	SCL_OUT_L();
	LC_IIC_Wait();
}
void	LC_IIC_Stop(void)
{
	SDA_OUT();
	SCL_OUT_L();
	SDA_OUT_L();
	LC_IIC_Wait();
	SCL_OUT_H();
	SDA_OUT_H();	//STOP:when CLK is high DATA change form low to high
	LC_IIC_Long_Wait();
}
/**
 * @brief	
 * 
 * @return uint8	1:ack failed, 0: ack success.
 */
uint8	LC_IIC_Wait_Ack(void)
{
	uint8	errTime	=	0;
	
	SDA_OUT();
	SDA_OUT_H();
	LC_IIC_Wait();
	SDA_IN();
	SCL_OUT_H();
	LC_IIC_Wait();
	while(SDA_READ() == 1)
	{
		errTime++;
		if(errTime > 255)
		{
			LC_IIC_Stop();
			return	1;
		}
	}
	SCL_OUT_L();
	return	0;
}
void	LC_IIC_Ack(void)
{
	SCL_OUT_L();
	SDA_OUT();
	SDA_OUT_L();
	LC_IIC_Wait();
	SCL_OUT_H();
	LC_IIC_Wait();
	SCL_OUT_L();
}
void	LC_IIC_NAck(void)
{
	SCL_OUT_L();
	SDA_OUT();
	SDA_OUT_H();
	LC_IIC_Wait();
	SCL_OUT_H();
	LC_IIC_Wait();
	SCL_OUT_L();
}
void	LC_IIC_Send_Byte(uint8 data)
{
	uint8	t;

	SDA_OUT();
	SCL_OUT_L();
	for(t=0;t<8;t++)
	{
		if((data&0x80))
		{
			SDA_OUT_H();
		}
		else
		{
			SDA_OUT_L();
		}
		data <<= 1;
		LC_IIC_Wait();
		SCL_OUT_H();
		LC_IIC_Wait();
		SCL_OUT_L();
		LC_IIC_Wait();
	}
}
uint8	LC_IIC_Read_Byte(uint8 ack)
{
	uint8	i, receive = 0;

	SDA_IN();
	for(i=0;i<8;i++)
	{
		SCL_OUT_L();
		LC_IIC_Wait();
		SCL_OUT_H();
		receive<<=1;
		if(SDA_READ() == 1)
		{
			receive++;
		}
		LC_IIC_Wait();
	}
	if(!ack)
	{
		LC_IIC_NAck();
	}
	else
	{
		LC_IIC_Ack();
	}
	return	receive;
}

/*!
 *	@fn			LC_Gpio_IIC_Init
 *	@brief		Initlialize the IIC pins.
 *	@param[in]	none.
 *	@return		none.
 */
void	LC_Gpio_IIC_Init(void)
{
	gpio_dir(GPIO_IIC_SDA, IE);
	gpio_pull_set(GPIO_IIC_SDA, STRONG_PULL_UP);

	gpio_dir(GPIO_IIC_SCL, OEN);
	// gpio_pull_set(GPIO_IIC_SCL, STRONG_PULL_UP);
}
/**
 * @brief	Deinitlialize.
 * 
 */
void	LC_Gpio_IIC_Deinit(void)
{
	gpio_dir(GPIO_IIC_SCL, IE);
	gpio_pull_set(GPIO_IIC_SCL, STRONG_PULL_UP);

	gpio_dir(GPIO_IIC_SDA, IE);
	gpio_pull_set(GPIO_IIC_SDA, STRONG_PULL_UP);
}
/*!
 *	@fn			LC_IIC_Read_Byte_Sequence.
 *	@brief		iic read continously.
 *	@param[in]	id:		device ID.
 *	@param[in]	addr:	initial address.
 *	@param[in]	p:		data buffer.
 *	@param[in]	len:	length of reading.
 *	@return	none.
 */
void	LC_IIC_Read_Byte_Sequence(uint8 id, uint8 addr, uint8 *p,uint8 len)
{
	LC_IIC_Start();

	LC_IIC_Send_Byte(id);
	LC_IIC_Send_Byte(addr);
	LC_IIC_Start();

	LC_IIC_Send_Byte(id |1);
	for(int k = 0;k<len;k++)
	{
		*p++ = LC_IIC_Read_Byte(k != (len -1));
	}
	LC_IIC_Stop();
}
/*!
 *	@fn			LC_IIC_Write_Byte_Sequence.
 *	@brief		iic write continously.
 *	@param[in]	id:		device ID.
 *	@param[in]	addr:	initial address.
 *	@param[in]	*p:		data buffer.
 *	@param[in]	len:	length of writing.
 *	@return	none.
 */
void	LC_IIC_Write_Byte_Sequence(uint8 id, uint8 addr, uint8 *p, uint8 len)
{
	LC_IIC_Start();
	LC_IIC_Send_Byte(id);
	LC_IIC_Send_Byte(addr);
	for(int i = 0;i<len;i++)
	{
		LC_IIC_Send_Byte(*p++);
	}
	LC_IIC_Stop();
}
/*!
 *	@fn			LC_IIC_Write_One_Byte.
 *	@brief		eeprom write one byte.
 *	@param[in]	id:		device ID.
 *	@param[in]	addr:	initial address.
 *	@param[in]	data:	.
 *	@return	none.
 */
void	LC_IIC_Write_One_Byte(uint8 id, uint8 addr, uint8 data)
{
	LC_IIC_Start();
	LC_IIC_Send_Byte(id);
	LC_IIC_Send_Byte(addr);
	LC_IIC_Send_Byte(data);
	LC_IIC_Stop();
}
/*!
 *	@fn			LC_IIC_Read_One_Byte.
 *	@brief		eeprom read one byte.
 *	@param[in]	id:		device ID.
 *	@param[in]	addr:	initial address.
 *	@return	data.
 */
uint8	LC_IIC_Read_One_Byte(uint8 id, uint8 addr)
{
	uint8 temp = 0;

	LC_IIC_Start();
	LC_IIC_Send_Byte(id);
	LC_IIC_Send_Byte(addr);
	LC_IIC_Start();
	LC_IIC_Send_Byte(id|1);
	temp	=	LC_IIC_Read_Byte(0);
	LC_IIC_Stop();
	return temp;
}
 /** @}*/

