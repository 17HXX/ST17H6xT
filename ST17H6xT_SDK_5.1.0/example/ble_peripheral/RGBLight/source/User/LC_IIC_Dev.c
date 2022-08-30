/**
 *	@file		LC_IIC_Dev.c
 *	@author		YQ(1500861686)
 *	@date		05/12/2022
 *	@brief		drivers of IIC Devices.
 */

/*!
 *	@defgroup	LC_IIC_Dev
 *	@brief
 *	@{*/

/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_IIC_Dev.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/**
 * @brief	Initilize of AT24CXX driver.
 * 
 */
void	AT24CXX_Init(void)
{
	LC_Gpio_IIC_Init();
}
/**
 * @brief	read data from AT24CXX at a specific address.
 * 
 * @param	ReadAdrr 	:specific address.
 * @return	uint8 		:wanted data.
 */
uint8	AT24CXX_ReadOneByte(uint8 ReadAdrr)
{
	uint8	temp=0;
	LC_IIC_Start();
	LC_IIC_Send_Byte(0xA0);
	LC_IIC_Wait_Ack();
	LC_IIC_Send_Byte(ReadAdrr);
	LC_IIC_Wait_Ack();
	LC_IIC_Start();
	LC_IIC_Send_Byte(0xA1);
	LC_IIC_Wait_Ack();
	temp	=	LC_IIC_Read_Byte(0);
	LC_IIC_Stop();
	return	temp;
}
/**
 * @brief	write data to AT24CXX at a specific address.
 * 
 * @param WriteAdrr		:specific address.
 * @param DataToWrite	:data wanted to write.
 */
void	AT24CXX_WriteOneByte(uint8 WriteAdrr, uint8 DataToWrite)
{
	LC_IIC_Start();
	LC_IIC_Send_Byte(0xA0);
	LC_IIC_Wait_Ack();
	LC_IIC_Send_Byte(WriteAdrr);
	LC_IIC_Wait_Ack();
	LC_IIC_Send_Byte(DataToWrite);
	LC_IIC_Wait_Ack();
	LC_IIC_Stop();
	WaitMs(10);
}
/**
 * @brief	write 16bit or 32bit data to AT24CXX at a specific address.
 * 
 * @param WriteAdrr		:specific address.
 * @param DataToWrite	:data pointer.
 * @param Len			:data byte length 2 or 4.
 */
void	AT24CXX_WriteLongByte(uint16 WriteAdrr, uint32 DataToWrite, uint8 Len)
{
	uint8 index;
	for(index = 0; index < Len; index++)
	{
		AT24CXX_WriteOneByte(WriteAdrr + index, (DataToWrite>>(8*index))&&0xff);
	}
}
/**
 * @brief	read 16bit or 32bit data from AT24CXX at a specific address.
 * 
 * @param ReadAddr		:specific address.
 * @param Len			:length of data 2 or 4.
 * @return uint32		:wanted data.
 */
uint32	AT24CXX_ReadLongByte(uint16 ReadAddr, uint8	Len)
{
	uint8	index;
	uint32	temp=0;
	for(index = 0; index < Len ;index++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr + Len - index - 1);
	}
	return	temp;
}
/**
 * @brief	Check AT24C02.
 * 
 * @return	uint8	:1 check failed, 0 check success.
 */
uint8	AT24CXX_Check(void)
{
	uint8	temp;
	temp	=	AT24CXX_ReadOneByte(255);
	if(temp == 0x55)
	{
		return	0;
	}
	else
	{
		AT24CXX_WriteOneByte(255, 0x55);
		temp	=	AT24CXX_ReadOneByte(255);
		if(temp == 0x55)
			return	0;
	}
	return	1;
}
/**
 * @brief	Read specific number data from AT24CXX at a specific address.
 * 
 * @param ReadAddr	:specific adderss.
 * @param pBuffer	:data buffer.
 * @param NumToRead	:wanted data number(AT24C02:0-255).
 */
void	AT24CXX_Read(uint8 ReadAddr, uint8* pBuffer, uint16 NumToRead)
{
	LC_IIC_Start();
	LC_IIC_Send_Byte(0xA0);
	LC_IIC_Wait_Ack();
	LC_IIC_Send_Byte(ReadAddr);
	LC_IIC_Wait_Ack();
	LC_IIC_Start();
	LC_IIC_Send_Byte(0xA1);
	LC_IIC_Wait_Ack();
	for(uint8 index = 0; index < NumToRead; index++)
	{
		*pBuffer++	=	LC_IIC_Read_Byte(index != (NumToRead - 1));
	}
	LC_IIC_Stop();
}
/**
 * @brief	Write specific number data to AT24CXX at a specific address.
 * 
 * @param WriteAddr	:specific adderss.
 * @param pBuffer	:data buffer.
 * @param NumToWrite:wanted data number(AT24C02:0-255).
 */
void	AT24CXX_Write(uint8 WriteAddr, uint8* pBuffer, uint16 NumToWrite)
{
	uint8	index;

	LC_IIC_Start();
	LC_IIC_Send_Byte(0xA0);
	LC_IIC_Wait_Ack();
	LC_IIC_Send_Byte(WriteAddr);
	LC_IIC_Wait_Ack();

	for(index = 0; index < NumToWrite; index++)
	{
		LC_IIC_Send_Byte(*pBuffer++);
		LC_IIC_Wait_Ack();
	}
	LC_IIC_Stop();

}
/** @}*/