/*
 * @Author: Ptisak
 * @Date: 2022-07-23 20:02:44
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-26 22:42:51
 * @Version: Do not edit
 */
#include "servo.h"
#define map(x, in_min, in_max, out_min, out_max) \
    (((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min))

void servo_set_angle(uint8_t n, float angle)
{
    float p = map(angle, 0.f, 180.f, 0.025f, 0.125f);
    uint16_t arr = p * TA1CCR0;
    // oled_printf_int32(0, 7, arr, 6);
    // send_four_data(0xf1, arr, 0, 0, 0);
    // PWM_CHANGE_CCR(1, 1, arr);
    TIMER_A_CMSIS(TIMER_A2_BASE)->CCR[n] = arr;
    // Timer_A_setCompareValue(TIMER_A1_BASE, n * 2 + 2, arr);
}
void gimbals_set(float angle1, float angle2)
{
    float p = map(angle1, -135.f, 135.f, 0.025f, 0.125f);
    uint16_t arr = p * TA1CCR0;
    TIMER_A_CMSIS(TIMER_A2_BASE)->CCR[1] = arr;
    p = map(angle2, -90.f, 90.f, 0.025f, 0.125f);
    arr = p * TA1CCR0;
    TIMER_A_CMSIS(TIMER_A2_BASE)->CCR[2] = arr;
}
