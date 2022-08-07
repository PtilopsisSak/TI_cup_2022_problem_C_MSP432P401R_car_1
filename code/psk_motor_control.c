/*
 * @Author: Ptisak
 * @Date: 2022-06-15 11:36:20
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-29 17:03:57
 * @Version: Do not edit
 */
#include "psk_motor_control.h"
#include "psk_ALL.h"
int encode_lb, encode_rb, encode_lf, encode_rf;
int32_t encode_100ms_total;
int64_t encode_total;
float basic_speed;
// 0-100
/**
 * @brief : 设置后侧车轮的pwm占空比
 * @note  : -100 ~ 100 复数反转
 * @param {float} l_speed
 * @param {float} r_speed
 */
void set_speed_back(float l_speed, float r_speed)
{
    if (r_speed < -0.03)
    {
        PWM_CHANGE_CCR(0, 1, -r_speed * TA0CCR0 / 100);
        PWM_CHANGE_CCR(0, 2, 0);
    }
    else if (r_speed > 0.03)
    {
        PWM_CHANGE_CCR(0, 1, 0);
        PWM_CHANGE_CCR(0, 2, r_speed * TA0CCR0 / 100);
    }
    else
    {
        PWM_CHANGE_CCR(0, 2, TA0CCR0);
        PWM_CHANGE_CCR(0, 1, TA0CCR0);
    }
    if (l_speed < -0.03)
    {
        PWM_CHANGE_CCR(0, 4, -l_speed * TA0CCR0 / 100);
        PWM_CHANGE_CCR(0, 3, 0);
    }
    else if (l_speed > 0.03)
    {
        PWM_CHANGE_CCR(0, 4, 0);
        PWM_CHANGE_CCR(0, 3, l_speed * TA0CCR0 / 100);
    }
    else
    {
        PWM_CHANGE_CCR(0, 3, TA0CCR0);
        PWM_CHANGE_CCR(0, 4, TA0CCR0);
    }
}
void set_speed_front(float l_speed, float r_speed)
{
    if (r_speed < -0.03)
    {
        PWM_CHANGE_CCR(1, 1, -r_speed * TA1CCR0 / 100);
        PWM_CHANGE_CCR(1, 2, 0);
    }
    else if (r_speed > 0.03)
    {
        PWM_CHANGE_CCR(1, 1, 0);
        PWM_CHANGE_CCR(1, 2, r_speed * TA1CCR0 / 100);
    }
    else
    {
        PWM_CHANGE_CCR(1, 2, TA1CCR0);
        PWM_CHANGE_CCR(1, 1, TA1CCR0);
    }
    if (l_speed < -0.03)
    {
        PWM_CHANGE_CCR(1, 4, -l_speed * TA1CCR0 / 100);
        PWM_CHANGE_CCR(1, 3, 0 * TA1CCR0);
    }
    else if (l_speed > 0.03)
    {
        PWM_CHANGE_CCR(1, 4, 0);
        PWM_CHANGE_CCR(1, 3, l_speed * TA1CCR0 / 100);
    }
    else
    {
        PWM_CHANGE_CCR(1, 3, TA1CCR0);
        PWM_CHANGE_CCR(1, 4, TA1CCR0);
    }
}

// cm / s
/**
 * @brief : 闭环控制
 * @note  :
 * @param {float} l_speed   左侧速度
 * @param {float} r_speed   右侧速度
 */
void pid_set(float l_speed, float r_speed)
{
    int encode_local_encode_lb = encode_lb;
    int encode_local_encode_rb = encode_rb;
    int encode_local_encode_lf = encode_lf;
    int encode_local_encode_rf = encode_rf;
    encode_lb = 0;
    encode_rb = 0;
    encode_lf = 0;
    encode_rf = 0;
    // send_four_data(0xf2, encode_lb, encode_rb, encode_lf, encode_rf);
    // return;
    // encode_lb, encode_rb, encode_lf, encode_rf
    //车跑1m脉冲数 1326 1337 1381 1363  average 1351.75
    //     * 1.09409f
    // * 1.09409f
    float l_tar = l_speed * 0.135175f * 688.f / 548;
    float r_tar = r_speed * 0.135175f * 688.f / 548;
    // float l_tar = -0;
    // float r_tar = 10;

    float speed_l, speed_r;
    speed_l = pid_calc(&pid_lb, encode_local_encode_lb, l_tar);
    speed_r = pid_calc(&pid_rb, encode_local_encode_rb, r_tar);
    // send_four_data(0xf1, speed_l, encode_local_encode_lb, l_tar, 0);
    // send_four_data(0xf2, speed_r, encode_local_encode_rb, r_tar, 0);
    if (basic_speed < 1)
    {
        if (l_tar - encode_local_encode_lb < -2)
        {
            speed_l = -100;
            pid_lf.iout = 0;
        }
        if (r_tar - encode_local_encode_rb < -2)
        {
            speed_r = -100;
            pid_rf.iout = 0;
        }
    }
    // send_four_data(0xf1, speed_l, encode_local_encode_lb, l_tar, 0);

    set_speed_back(speed_l, speed_r);

    speed_l = pid_calc(&pid_lf, encode_local_encode_lf, l_tar);
    speed_r = pid_calc(&pid_rf, encode_local_encode_rf, r_tar);
    //    send_four_data(0xf1, speed_l, encode_local_encode_lf, l_tar, 0);
    //    send_four_data(0xf2, speed_r, encode_local_encode_rf, r_tar, 0);
    if (basic_speed < 1)
    {
        if (l_tar - encode_local_encode_lf < -2)
        {
            speed_l = -100;
            pid_lf.iout = 0;
        }
        if (r_tar - encode_local_encode_rf < -2)
        {
            speed_r = -100;
            pid_rf.iout = 0;
        }
    }
    encode_100ms_total += encode_local_encode_lb;
    encode_100ms_total += encode_local_encode_rb;
    encode_100ms_total += encode_local_encode_lf;
    encode_100ms_total += encode_local_encode_rf;

    encode_total += encode_local_encode_lb;
    encode_total += encode_local_encode_rb;
    encode_total += encode_local_encode_lf;
    encode_total += encode_local_encode_rf;
    set_speed_front(speed_l, speed_r);
}

float distance_calc(void)
{
    return encode_total * 0.0184945441094877f / 688.f * 548;
}

void until_distance(float cm)
{
    // float start_diatance = distance_calc();
    float end_diatance = distance_calc() + cm;
    while (1)
    {
        delay_ms(5);
        if (distance_calc() > end_diatance)
            break;
    }
}