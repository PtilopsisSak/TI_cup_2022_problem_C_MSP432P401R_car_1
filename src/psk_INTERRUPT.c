/*
 * @Author: Ptisak
 * @Date: 2022-07-20 14:32:48
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-31 09:52:01
 * @Version: Do not edit
 */

#include "psk_INTERRUPT.h"
#include "psk_ALL.h"
/* GPIO ISR */
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    switch (status)
    {
    case GPIO_PIN0:
        break;
    case GPIO_PIN1:
        // MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        break;
    case GPIO_PIN2:
        break;
    case GPIO_PIN3:
        break;
    case GPIO_PIN4:
        break;
    case GPIO_PIN5:
        break;
    case GPIO_PIN6:
        break;
    case GPIO_PIN7:
        break;
    }
}
void PORT5_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    switch (status)
    {
    case GPIO_PIN0:
        break;
    case GPIO_PIN1:
        break;
    case GPIO_PIN2:
        break;
    case GPIO_PIN3:
        break;
    case GPIO_PIN4:
        break;
    case GPIO_PIN5:
        break;
    case GPIO_PIN6:
        break;
    case GPIO_PIN7:
        break;
    }
}
void PORT4_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    switch (status)
    {
    case GPIO_PIN0:
        if (GPIO_READ(4, 1))
        {
            encode_lf++;
        }
        else
        {
            encode_lf--;
        }

        break;
    case GPIO_PIN1:
        if (!GPIO_READ(4, 0))
        {
            encode_lf++;
        }
        else
        {
            encode_lf--;
        }

        break;
    case GPIO_PIN2:
        if (GPIO_READ(4, 3))
        {
            encode_rf++;
        }
        else
        {
            encode_rf--;
        }

        break;
    case GPIO_PIN3:
        if (!GPIO_READ(4, 2))
        {
            encode_rf++;
        }
        else
        {
            encode_rf--;
        }

        break;
    case GPIO_PIN4:
        if (GPIO_READ(4, 5))
        {
            encode_lb++;
        }
        else
        {
            encode_lb--;
        }

        break;
    case GPIO_PIN5:
        if (!GPIO_READ(4, 4))
        {
            encode_lb++;
        }
        else
        {
            encode_lb--;
        }

        break;
    case GPIO_PIN6:
        if (GPIO_READ(4, 7))
        {
            encode_rb++;
        }
        else
        {
            encode_rb--;
        }
        // MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        break;
    case GPIO_PIN7:
        if (!GPIO_READ(4, 6))
        {
            encode_rb++;
        }
        else
        {
            encode_rb--;
        }

        break;
    }
}
/* Timer A3 IRQ Hander */
void TA3_0_IRQHandler(void)
{
    static int timer;
    static float clac_speed, control_speed, disLoop_speed = 0;
    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0); //清除中断状态
    Timer_A_clearTimer(TIMER_A3_BASE);                                                      //清除定时器计数
                                                                                            // get_pos();
    // GPIO_TOGGLE(1, 0);
    led_lignt_ctl();
    buzzer_bbbi();
    if (time_count_flag)
    {
        mission_time_count++;
    }

    timer++;
    if (timer > 9)
    {
        timer = 0;
        clac_speed = calc_my_speed();
        control_speed = basic_speed;
        // control_speed = pid_calc(&pid_speed, clac_speed, basic_speed);
        // send_four_data(0xf3, clac_speed * 100, control_speed * 100, basic_speed * 100, huidu_error * 100);

        // zf_sprintf(printf_temp, "%d.%d s", mission_time_count / 100, mission_time_count % 100);
        // OLED_ShowString(0, 5, printf_temp, 8);

        // zf_sprintf(printf_temp, "%d cm  ", (int)distance_calc());
        // OLED_ShowString(0, 6, printf_temp, 8);
        disLoop_speed = calc_disLoop_speed(calc_distance(ultrasonic_distance, 0, 0));
    }
    control_speed = basic_speed + disLoop_speed;

    huidu_get_error();
    if (basic_speed < 0.03)
    {
        huidu_error = 0;
    }
    else if (huifdu_flags.trace_mode == NONE)
    {
        huifdu_flags.trace_mode = RIGHT_FIRST;
    }
    pid_set(control_speed + huidu_error, control_speed - huidu_error);
    // send_four_data(0xf2, encode_lb, encode_rb, encode_lf, encode_rf);
    // send_four_data(0xf1, GPIO_READ(10, 0), GPIO_READ(10, 1), GPIO_READ(10, 2), GPIO_READ(10, 3));
    // send_four_data(0xf3, 0, damping * 100, 0, 0);
    // printf("%d\r\n", (int)damping * 100);
    // zf_sprintf(printf_temp, "%d", (int)damping * 100);
    // OLED_ShowString(0, 5, printf_temp, 8);
    // send_four_data(0xf3, clac_speed * 100, control_speed * 100, basic_speed * 100, huidu_error * 100);
}

/**
 * @brief :下降沿触发
 * @note  :GPIO_INIT_IT(1, 0);
 * @param {int} port
 * @param {int} pin
 */
void GPIO_INIT_IT(uint8_t port, uint8_t pin)
{
    if (pin > 7)
        return;

    MAP_GPIO_setAsInputPinWithPullUpResistor(port, (1 << pin));
    MAP_GPIO_clearInterruptFlag(port, (1 << pin));
    MAP_GPIO_enableInterrupt(port, (1 << pin));
    MAP_Interrupt_enableInterrupt(50 + port);
    P4IES &= ~BIT1; // up
    P4IES |= 0xff;  // down
}
/* Application Defines  */

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_48,     // SMCLK/48 = 1us
        25000,                              //
        TIMER_A_TAIE_INTERRUPT_DISABLE,     // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                    // Clear value
};

void TA3_IT_INIT(void)
{
    Timer_A_configureUpMode(TIMER_A3_BASE, &upConfig);
    // Interrupt_enableSleepOnIsrExit();//退出中断进入低功耗模式 没必要
    Interrupt_enableInterrupt(INT_TA3_0);
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);

    TA3CCR0 = 10000;
    //	//开中断控制器
    Interrupt_enableMaster();
}