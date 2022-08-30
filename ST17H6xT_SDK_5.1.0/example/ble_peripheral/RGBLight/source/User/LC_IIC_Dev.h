/**
 *	@file		LC_IIC_Dev.h
 *	@author		YQ(1500861686)
 *	@date		05/12/2022
 *	@version	1.0.0
 */

/*!
 *	@defgroup	LC_IIC_Dev
 *	@brief
 *	@{*/

#ifndef		LC_IIC_DEV_H_
#define		LC_IIC_DEV_H_
/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef	__cplusplus
	extern "C"	{
#endif
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_IIC.h"
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
//	EEPROM AT24C02 Address Configuration
#define		EEPROM_RGB_MODE_DATA	0x00
#define		EEPROM_RGB_ALARM_DATA	0x20
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
/**
 * @brief	Initilize of AT24CXX driver.
 * 
 */
void	AT24CXX_Init(void);
/**
 * @brief	read data from AT24CXX at a specific address.
 * 
 * @param	ReadAdrr 	:specific address.
 * @return	uint8 		:wanted data.
 */
uint8	AT24CXX_ReadOneByte(uint8 ReadAdrr);

/**
 * @brief	write data to AT24CXX at a specific address.
 * 
 * @param WriteAdrr		:specific address.
 * @param DataToWrite	:data wanted to write.
 */
void	AT24CXX_WriteOneByte(uint8 WriteAdrr, uint8 DataToWrite);

/**
 * @brief	write 16bit or 32bit data to AT24CXX at a specific address.
 * 
 * @param WriteAdrr		:specific address.
 * @param DataToWrite	:data pointer.
 * @param Len			:data byte length 2 or 4.
 */
void	AT24CXX_WriteLongByte(uint16 WriteAdrr, uint32 DataToWrite, uint8 Len);

/**
 * @brief	read 16bit or 32bit data from AT24CXX at a specific address.
 * 
 * @param ReadAddr		:specific address.
 * @param Len			:length of data 2 or 4.
 * @return uint32		:wanted data.
 */
uint32	AT24CXX_ReadLongByte(uint16 ReadAddr, uint8	Len);

/**
 * @brief	Check AT24C02.
 * 
 * @return	uint8	:1 check failed, 0 check success.
 */
uint8	AT24CXX_Check(void);

/**
 * @brief	Read specific number data from AT24CXX at a specific address.
 * 
 * @param ReadAddr	:specific adderss.
 * @param pBuffer	:data buffer.
 * @param NumToRead	:wanted data number(AT24C02:0-255).
 */
void	AT24CXX_Read(uint8 ReadAddr, uint8* pBuffer, uint16 NumToRead);

/**
 * @brief	Write specific number data to AT24CXX at a specific address.
 * 
 * @param WriteAddr	:specific adderss.
 * @param pBuffer	:data buffer.
 * @param NumToWrite:wanted data number(AT24C02:0-255).
 */
void	AT24CXX_Write(uint8 WriteAddr, uint8* pBuffer, uint16 NumToWrite);
#ifdef	__cplusplus
	}
#endif

#endif	/**	LC_IIC_Dev.h **/
/**	@}*/