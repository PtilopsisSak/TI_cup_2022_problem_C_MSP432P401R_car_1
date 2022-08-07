/*
 * @Author: Ptisak
 * @Date: 2022-07-23 20:02:44
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-28 17:26:16
 * @Version: Do not edit
 */
#include "led_beep.h"
int buzzer_bi_flag;

/**
 * @brief : 间隔闪灯, 单位ms, 但是最小10ms, 间隔10, 奇数交替, 偶数同时
 * @note  :
 */
int led_blink_flag;
int time_count_flag;
void buzzer_bbbi(void)
{
    static int buzzer_count = 0;
    if (buzzer_bi_flag == 1)
    {
        BEEP_ON;
        buzzer_count++;
    }

    if (buzzer_count > 15)
    {
        BEEP_OFF;
        buzzer_count = 0;
        buzzer_bi_flag = 0;
    }
}

void led_lignt_ctl(void)
{
    static int led_count = 0;
    if (led_blink_flag)
    {
        if (led_blink_flag & 0x01)
        {
            LED_TOGGLE;
            led_blink_flag--;
        }
        led_count += 10;
        if (led_count > led_blink_flag)
        {
            led_count = 0;
            LED_TOGGLE;
            LED1_TOGGLE;
        }
    }
    else
    {
        LED_OFF;
        LED1_OFF;
    }
}
void time_count(void)
{
}