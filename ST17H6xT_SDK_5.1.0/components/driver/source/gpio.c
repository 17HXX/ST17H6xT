/**************************************************************************************************
**************************************************************************************************/

/**************************************************************************************************
  Filename:       gpio_demo.c
  Revised:
  Revision:

  Description:


**************************************************************************************************/
/*********************************************************************
 * INCLUDES
 */
#include "rom_sym_def.h"
#include "global_config.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "jump_function.h"
#include  "log.h"
#include "pwrmgr.h"
/*********************************************************************
 * MACROS
 */
// #define LOG(...)
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************a************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
extern void gpio_sleep_handler(void);
extern void gpioin_event_pin(gpioin_t *p_gpioin_ctx, gpio_polarity_e type);
extern void gpio_retention_active(gpio_pin_e pin, bool en);
extern int pwrmgr_register(MODULE_e mod, pwrmgr_Hdl_t sleepHandle, pwrmgr_Hdl_t wakeupHandle);
extern int pwrmgr_unregister(MODULE_e mod);

extern gpio_Ctx_t m_gpioCtx;

typedef struct{
    uint8_t    reg_i;
    uint8_t    bit_h;
    uint8_t    bit_l;
}gpio_pupd_cfg_t;

typedef struct{
  uint8_t idx;
  gpio_pupd_cfg_t pupd;
  uint8_t retention[2];
}pinmap_t;

extern pinmap_t s_gpio_pinmap[GPIO_NORMAL + 1];
extern uint32_t s_gpio_wakeup_src[2];

uint32_t analog_map;

void hal_gpio_IRQ(void)
{
    _symrom_GPIO_IRQHandler();
}

void gpioin_wakeup_trigger_patch(gpioin_t *p_gpioin_ctx)
{
    gpio_polarity_e type = p_gpioin_ctx->pin_state ? POL_FALLING : POL_RISING;
    int pin_state = gpio_read(p_gpioin_ctx->pin);

    gpioin_event_pin(p_gpioin_ctx, type);

    // below means gpio toggle is very shot, when wakeuped,
    // the pin level have been same as before sleeping
    // so we need make up the missed edge
    if(p_gpioin_ctx->pin_state == pin_state){
        type = pin_state ? POL_FALLING : POL_RISING; //order reverted!!!
        gpioin_event_pin(p_gpioin_ctx, type);
    }
}
void gpio_wakeup_handler_patch(void)
{
    int i;

    gpioin_t *p_gpioin_ctx = m_gpioCtx.gpioin_ctx;
    NVIC_SetPriority(GPIO_IRQn, IRQ_PRIO_HAL);
    NVIC_EnableIRQ(GPIO_IRQn);

    AP_IOMUX->Analog_IO_en = analog_map;

    for (i = 0; i <= GPIO_NORMAL; i++)
    {
        if ((i == 2) || (i == 3))
            gpio_pin2pin3_control((gpio_pin_e)i, 1);

        if (m_gpioCtx.retention_map & BIT(i))
        {
            bool pol = gpio_read((gpio_pin_e)i);
            gpio_write((gpio_pin_e)i, pol);
        }
        gpio_retention_active((gpio_pin_e)i, false);
    }

    // get wakeup source

    for (i = 0; i < m_gpioCtx.gpioin_nums; i++)
    {
        if (p_gpioin_ctx[i].enable)
        {
            gpioin_enable(p_gpioin_ctx + i); // resume gpio irq
            uint8_t pin_idx = s_gpio_pinmap[p_gpioin_ctx[i].pin].idx;
            uint32_t triggered = s_gpio_wakeup_src[pin_idx / 32] & BIT(pin_idx % 32);
            if (triggered)
                gpioin_wakeup_trigger_patch(p_gpioin_ctx + i); // trigger gpio irq manually
        }
    }
}
void gpio_sleep_handler_patch(void)
{
    int i;
    gpioin_t *p_gpioin_ctx = m_gpioCtx.gpioin_ctx;
    gpio_polarity_e pol;

    for (i = 0; i <= GPIO_NORMAL; i++)
    {
        // config retenstion setting
        if (m_gpioCtx.retention_map & BIT(i))
        {
            gpio_retention_active((gpio_pin_e)i, true);
        }
    }

    // config wakeup setting
    for (i = 0; i < m_gpioCtx.gpioin_nums; i++)
    {
        pol = gpio_read(p_gpioin_ctx[i].pin) ? POL_FALLING : POL_RISING;
        gpio_wakeup_set(p_gpioin_ctx[i].pin, pol);
        m_gpioCtx.gpioin_ctx[i].pin_state = gpio_read(p_gpioin_ctx[i].pin);
    }
    analog_map = AP_IOMUX->Analog_IO_en;
}
int gpio_init(void)
{
    volatile int ret;
    //	pGlobal_config[MAX_SLEEP_TIME] = 150000000;
    JUMP_FUNCTION_SET(GPIO_IRQ_HANDLER, (uint32_t)hal_gpio_IRQ);

    osal_memset(&m_gpioCtx, 0, sizeof(gpio_Ctx_t));

    ret = rom_gpio_init();
    pwrmgr_unregister(MOD_GPIO);
    pwrmgr_register(MOD_GPIO, gpio_sleep_handler_patch, gpio_wakeup_handler_patch);
    return ret;
}