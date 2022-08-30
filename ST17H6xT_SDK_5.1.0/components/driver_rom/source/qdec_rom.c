/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/

/*******************************************************************************
    @file     qdec.c
    @brief    Contains all functions support for key scan driver
    @version  0.0
    @date     13. Nov. 2017
    @author   Ding



*******************************************************************************/
//#include "rom_sym_def.h"
#include <string.h>
#include "bus_dev.h"

#include "OSAL.h"
#include "qdec.h"
#include "gpio.h"
#include "uart.h"
#include "log.h"
#include "pwrmgr.h"
#include "error.h"
#include "clock.h"


qdec_Ctx_t m_qdecCtx;


void __attribute__((used)) QDEC_IRQHandler()
{
    //gpio_dir(P20, GPIO_OUTPUT);
    //gpio_write(P20,1);
    osal_stop_timerEx(m_qdecCtx.qdec_task_id, m_qdecCtx.timeout_event);
    WaitMs(1);
    int32_t delta = GET_CNT_QUAN(m_qdecCtx.cfg.qdec_chn);
    m_qdecCtx.count += delta;

    if(m_qdecCtx.cfg.evt_handler)
    {
        qdec_Evt_t evt;
        evt.count = m_qdecCtx.count;
        m_qdecCtx.cfg.evt_handler(&evt);
    }

    CLR_INT_QUAN(m_qdecCtx.cfg.qdec_chn);
    pwrmgr_unlock(MOD_QDEC);
	//gpio_dir(P20, GPIO_OUTPUT);
	//gpio_write(P20,0);
}

void qdec_timeout_handler()
{
    osal_stop_timerEx(m_qdecCtx.qdec_task_id, m_qdecCtx.timeout_event);
    pwrmgr_unlock(MOD_QDEC);
}

/**************************************************************************************
    @fn          hal_qdec_set_cha

    @brief       This function process for qdec initial

    input parameters

    @param       None.

    output parameters

    @param       None.

    @return      None.
 **************************************************************************************/
static void qdec_set_cha(QDEC_CHN_e qdecCHN,gpio_pin_e pin)
{
    gpio_pull_set(pin, PULL_DOWN);
    gpio_fmux_set(pin, (gpio_fmux_e)(FMUX_CHAX + (qdecCHN*3)));
}

/**************************************************************************************
    @fn          hal_qdec_set_chb

    @brief       This function process for qdec initial

    input parameters

    @param       None.

    output parameters

    @param       None.

    @return      None.
 **************************************************************************************/
static void qdec_set_chb(QDEC_CHN_e qdecCHN,gpio_pin_e pin)
{
    gpio_pull_set(pin, PULL_DOWN);
    gpio_fmux_set(pin, (gpio_fmux_e)(FMUX_CHBX + (qdecCHN*3)));
}

/**************************************************************************************
    @fn          hal_qdec_set_chi

    @brief       This function process for qdec initial

    input parameters

    @param       None.

    output parameters

    @param       None.

    @return      None.
 **************************************************************************************/
static void qdec_set_chi(QDEC_CHN_e qdecCHN,gpio_pin_e pin)
{
    gpio_pull_set(pin, PULL_DOWN);
    gpio_fmux_set(pin, (gpio_fmux_e)(FMUX_CHIX + (qdecCHN*3)));
}

/**************************************************************************************
    @fn          hal_qdec_init

    @brief       This function process for qdec initial

    input parameters

    @param       qdec_Cfg_t cfg

    output parameters

    @param       None.

    @return      None.
 **************************************************************************************/
int qdec_init(qdec_Cfg_t cfg, uint8 task_id, uint16 event)
{
    if(m_qdecCtx.enable)
        return PPlus_ERR_INVALID_STATE;

    uint8_t pins[3] = {cfg.cha_pin, cfg.chb_pin, cfg.chi_pin};
    memcpy(m_qdecCtx.pin_arr, pins, sizeof(uint8_t)*3);
    m_qdecCtx.cfg = cfg;
    m_qdecCtx.qdec_task_id = task_id;
    m_qdecCtx.timeout_event = event;
    m_qdecCtx.enable = TRUE;
    qdec_hw_config();
    pwrmgr_register(MOD_QDEC, qdec_sleep_handler, qdec_wakeup_handler);
    return PPlus_SUCCESS;
}

void qdec_hw_config()
{
    qdec_Cfg_t* cfg = &(m_qdecCtx.cfg);
	
    clk_gate_enable(MOD_QDEC);
    qdec_set_cha(cfg->qdec_chn, cfg->cha_pin);
    qdec_set_chb(cfg->qdec_chn, cfg->chb_pin);
	
    DIS_INT_INCN(cfg->qdec_chn);
    DIS_INT_QUAN(cfg->qdec_chn);
    DIS_INT_02F_QUAN(cfg->qdec_chn);
    DIS_INT_F20_QUAN(cfg->qdec_chn);
    SET_MODE_QUAN(cfg->qdec_chn, cfg->quaMode);
	
    qdec_set_qua_irq(cfg->qdec_chn, cfg->intMode);

    if(cfg->use_inc)
    {
        qdec_set_chi(cfg->qdec_chn, cfg->chi_pin);

        if(cfg->use_inc_irq)
        {
            qdec_set_inc_irq(cfg->qdec_chn, cfg->incMode, cfg->intMode);
        }
    }

    QDEC_IRQ_ENABLE;
    ENABLE_CHN(cfg->qdec_chn);
}

static void qdec_sleep_handler(void)
{
    uint8_t pin_num;
    pin_num = m_qdecCtx.cfg.use_inc ? 3:2;

    for(uint8_t i=0; i<pin_num; i++)
    {
        gpio_polarity_e pol;
        gpio_pin_e pin = m_qdecCtx.pin_arr[i];
		
        gpio_dir(pin, GPIO_INPUT);
        pol = gpio_read(pin) ? POL_FALLING:POL_RISING;
        gpio_wakeup_set(pin, pol);
        m_qdecCtx.pin_state[i] = pol;
    }

    DISABLE_CHN(m_qdecCtx.cfg.qdec_chn);
    //gpio_dir(P23, GPIO_OUTPUT);
    //gpio_write(P23,0);
}

static void qdec_wakeup_handler(void)
{
    //gpio_dir(P23, GPIO_OUTPUT);
    //gpio_write(P23,1);
    uint8_t pin_num;
    pin_num = m_qdecCtx.cfg.use_inc ? 3:2;
    gpio_polarity_e pol;

    for(uint8_t i=0; i<pin_num; i++)
    {
        gpio_dir(m_qdecCtx.pin_arr[i], GPIO_INPUT);
        pol = gpio_read(m_qdecCtx.pin_arr[i]) ? POL_RISING :POL_FALLING;

        if(pol == m_qdecCtx.pin_state[i])
        {
            break;
        }
        else if(i == pin_num-1)
        {
            return;
        }
    }

    int32_t delta = GET_CNT_QUAN(m_qdecCtx.cfg.qdec_chn);
    //gpio_dir(P31, GPIO_OUTPUT);
    //gpio_write(P31,0);
    pwrmgr_lock(MOD_QDEC);
    qdec_hw_config();
    //gpio_dir(P31, GPIO_OUTPUT);
    //gpio_write(P31,1);
    osal_start_timerEx(m_qdecCtx.qdec_task_id, m_qdecCtx.timeout_event, 150);
}

/**************************************************************************************
    @fn          hal_qdec_set_qua_irq

    @brief       This function process for setting qdecode counter interupt mode

    input parameters

    @param       QDEC_CHN_e chn
    @param       QDEC_INT_MODE_e intMode

    output parameters

    @param       None.

    @return      None.
 **************************************************************************************/
static void qdec_set_qua_irq(QDEC_CHN_e chn, QDEC_INT_MODE_e intMode)
{
    SET_INT_MODE_QUAN(chn, intMode);
    EN_INT_QUAN(chn);
}

/**************************************************************************************
    @fn          hal_qdec_set_inc_irq

    @brief       This function process for setting qdecode index count mode and interupt mode

    input parameters

    @param       QDEC_CHN_e chn
    @param       QDEC_INT_MODE_e intMode
    @param       QDEC_INC_MODE_e incMode

    output parameters

    @param       None.

    @return      None.
 **************************************************************************************/
static void qdec_set_inc_irq(QDEC_CHN_e chn, QDEC_INC_MODE_e incMode, QDEC_INT_MODE_e intMode)
{
    SET_MODE_INCN(chn, incMode);
    SET_INT_MODE_INCN(chn, intMode);
    EN_INT_INCN(chn);
}
