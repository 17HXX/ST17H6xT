/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/*******************************************************************************
* @file		pwm.h
* @brief	Contains all functions support for pwm driver
* @version	0.0
* @date		30. Oct. 2017
* @author	Ding
* 
* 
* 
*
*******************************************************************************/
#ifndef __PWM_ROM_H__
#define __PWM_ROM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "bus_dev.h"
#include "gpio.h"


#define    PWM_CH_BASE              (AP_PWM_BASE + 4)
#define    PWM_CTL0_ADDR(n)        	(PWM_CH_BASE + n*12)
#define    PWM_CTL1_ADDR(n)        	(PWM_CH_BASE + n*12 + 4)

#define    PWM_ENABLE_ALL          	do{\
                                        AP_PWM->PWMEN |= BIT(0);\
                                        AP_PWM->PWMEN |= BIT(4);\
									}while(0)
#define    PWM_DISABLE_ALL 			do{\
                                        AP_PWM->PWMEN &= ~BIT(0);\
                                        AP_PWM->PWMEN &= ~BIT(4);\
									}while(0)

#define    PWM_ENABLE_CH_012      	do{\
										AP_PWM->PWMEN |= BIT(8);\
										AP_PWM->PWMEN |= BIT(9);\
									}while(0)
#define    PWM_DISABLE_CH_012       do{\
										AP_PWM->PWMEN &= ~BIT(8);\
										AP_PWM->PWMEN &= ~BIT(9);\
									}while(0)
#define    PWM_ENABLE_CH_345    	do{\
										AP_PWM->PWMEN |= BIT(10);\
										AP_PWM->PWMEN |= BIT(11);\
									}while(0)
#define    PWM_DISABLE_CH_345      	do{\
										AP_PWM->PWMEN &= ~BIT(10);\
										AP_PWM->PWMEN &= ~BIT(11);\
									}while(0)
#define    PWM_ENABLE_CH_01        	do{\
										AP_PWM->PWMEN |= BIT(12);\
										AP_PWM->PWMEN |= BIT(13);\
									}while(0)
#define    PWM_DISABLE_CH_01       	do{\
										AP_PWM->PWMEN &= ~BIT(12);\
										AP_PWM->PWMEN &= ~BIT(13);\
									}while(0)
#define    PWM_ENABLE_CH_23        	do{\
										AP_PWM->PWMEN |= BIT(14);\
										AP_PWM->PWMEN |= BIT(15);\
									}while(0)
#define    PWM_DISABLE_CH_23       	do{\
										AP_PWM->PWMEN &= ~BIT(14);\
										AP_PWM->PWMEN &= ~BIT(15);\
									}while(0)
#define    PWM_ENABLE_CH_45        	do{\
										AP_PWM->PWMEN |= BIT(16);\
										AP_PWM->PWMEN |= BIT(17);\
									}while(0)
#define    PWM_DISABLE_CH_45    	do{\
										AP_PWM->PWMEN &= ~BIT(16);\
										AP_PWM->PWMEN &= ~BIT(17);\
									}while(0)

						
#define    PWM_INSTANT_LOAD_CH(n)  	 subWriteReg(PWM_CTL0_ADDR(n),31,31,1)
#define    PWM_NO_INSTANT_LOAD_CH(n) subWriteReg(PWM_CTL0_ADDR(n),31,31,0)
#define    PWM_LOAD_CH(n) 			 subWriteReg(PWM_CTL0_ADDR(n),16,16,1)
#define    PWM_NO_LOAD_CH(n) 		 subWriteReg(PWM_CTL0_ADDR(n),16,16,0)																 
#define    PWM_SET_DIV(n,v) 		 subWriteReg(PWM_CTL0_ADDR(n),14,12,v)
#define    PWM_SET_MODE(n,v) 	   	 subWriteReg(PWM_CTL0_ADDR(n),8,8,v)
#define    PWM_SET_POL(n,v) 		 subWriteReg(PWM_CTL0_ADDR(n),4,4,v)
#define    PWM_ENABLE_CH(n)        	 subWriteReg(PWM_CTL0_ADDR(n),0,0,1)
#define    PWM_DISABLE_CH(n)       	 subWriteReg(PWM_CTL0_ADDR(n),0,0,0)
									
#define    PWM_SET_CMP_VAL(n,v) 	 subWriteReg(PWM_CTL1_ADDR(n),31,16,v)					
#define    PWM_SET_TOP_VAL(n,v) 	 subWriteReg(PWM_CTL1_ADDR(n),15,0,v)
#define    PWM_GET_CMP_VAL(n) 	   	 ((read_reg(PWM_CTL1_ADDR(n)) & 0xFFFF0000) >> 8)
#define    PWM_GET_TOP_VAL(n) 	   	 read_reg(PWM_CTL1_ADDR(n)) & 0x0000FFFF


/*************************************************************
*	@brief		enum variable, the number of PWM channels supported
*
*/
typedef enum
{
    PWM_CH0 = 0,
    PWM_CH1 = 1,
    PWM_CH2 = 2,
    PWM_CH3 = 3,
    PWM_CH4 = 4,
    PWM_CH5 = 5
} PWMN_e;

/*************************************************************
*	@brief		enum variable used for PWM clock prescaler
*
*/
typedef enum
{
    PWM_CLK_NO_DIV = 0,
    PWM_CLK_DIV_2 = 1,
    PWM_CLK_DIV_4 = 2,
    PWM_CLK_DIV_8 = 3,
    PWM_CLK_DIV_16 = 4,
    PWM_CLK_DIV_32 = 5,
    PWM_CLK_DIV_64 = 6,
    PWM_CLK_DIV_128 = 7
} PWM_CLK_DIV_e;

/*************************************************************
*	@brief		enum variable used for PWM work mode setting
*
*/
typedef enum
{
    PWM_CNT_UP = 0,
    PWM_CNT_UP_AND_DOWN = 1
} PWM_CNT_MODE_e;

/*************************************************************
*	@brief		enum variable used for PWM output polarity setting
*
*/
typedef enum
{
    PWM_POLARITY_RISING = 0,
    PWM_POLARITY_FALLING = 1
} PWM_POLARITY_e;

/**************************************************************************************
 * @fn          pwm_init
 *
 * @brief       This function process for pwm initial
 *
 * input parameters
 *
 * @param       PWMN_e pwmN						: pwm channel
 *              PWM_CLK_DIV_e pwmDiv			: clock prescaler of PWM channel
 *				PWM_CNT_MODE_e pwmMode			: count mode of PWM channel
 *				PWM_POLARITY_e pwmPolarity		: output polarity setting of PWM channel
 *				unsigned short cmpVal			: the compare value of PWM channel 
 *				unsigned short cntTopVal		: the counter top value of PWM channel
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
void pwm_init(PWMN_e pwmN, PWM_CLK_DIV_e pwmDiv,
	PWM_CNT_MODE_e pwmMode, PWM_POLARITY_e pwmPolarity);

/**************************************************************************************
 * @fn          pwm_open_channel
 *
 * @brief       This function process for pwm start working
 *
 * input parameters
 *
 * @param       PWMN_e pwmN						: pwm channel
 *              gpio_pin_e pwmPin				: pwm pin number
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
void pwm_open_channel(PWMN_e pwmN,gpio_pin_e pwmPin);

/**************************************************************************************
 * @fn          hal_pwm_close_channel
 *
 * @brief       This function process for pwm stop working
 *
 * input parameters
 *
 * @param       PWMN_e pwmN						: pwm channel
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
void pwm_close_channel(PWMN_e pwmN);

/**************************************************************************************
 * @fn          pwm_destroy
 *
 * @brief       This function process for pwm clear and disable
 *
 * input parameters
 *
 * @param       PWMN_e pwmN						: pwm channel
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
void pwm_destroy(PWMN_e pwmN);

/**************************************************************************************
 * @fn          pwm_set_count_val
 *
 * @brief       This function process for change pwm count value
 *
 * input parameters
 *
 * @param       PWMN_e pwmN						: pwm channel
 *				uint16_t cmpVal					: the compare value of PWM channel 
 *				uint16_t cntTopVal				: the counter top value of PWM channel
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
void pwm_set_count_val(PWMN_e pwmN, uint16_t cmpVal, uint16_t cntTopVal);

/**************************************************************************************
    @fn          pwm_start

    @brief       pwm start

    input parameters

    @param       None.

    output parameters

    @param       None.

    @return      None.
 **************************************************************************************/
void pwm_start(void);
/**************************************************************************************
    @fn          pwm_stop

    @brief       pwm stop

    input parameters

    @param       None.

    output parameters

    @param       None.

    @return      None.
 **************************************************************************************/
void pwm_stop(void);

//new api,make use easily
typedef struct
{  
	PWMN_e pwmN;
	gpio_pin_e pwmPin;
	PWM_CLK_DIV_e pwmDiv;
	PWM_CNT_MODE_e pwmMode;
	PWM_POLARITY_e pwmPolarity;
	uint16_t cmpVal;
	uint16_t cntTopVal;
  
} pwm_ch_t;

/**************************************************************************************
 * @fn          pwm_module_init
 *
 * @brief       init pwm global variables
 *
 * input parameters
 *
 * @param       None.      
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
void pwm_module_init(void);

/**************************************************************************************
 * @fn          pwm_module_deinit
 *
 * @brief       deinit pwm global variables
 *
 * input parameters
 *
 * @param       None.      
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
void pwm_module_deinit(void);

/**************************************************************************************
 * @fn          pwm_ch_start
 *
 * @brief       config and make a pwm start to work 
 *
 * input parameters
 *
 * @param       pwm_ch_t ch: pwm channel
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
void pwm_ch_start(pwm_ch_t ch);

/**************************************************************************************
 * @fn          pwm_ch_stop
 *
 * @brief       make a pwm stop form working
 *
 * input parameters
 *
 * @param       pwm_ch_t ch: pwm channel
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
void pwm_ch_stop(pwm_ch_t ch);

bool pwm_ch_enable(PWMN_e pwmN);

pwm_ch_t pwm_ch_reg(PWMN_e pwmN);

#ifdef __cplusplus
}
#endif

#endif
