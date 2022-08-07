/*
 * @Author: ksn
 * @Date: 2020-10-08 15:27:43
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-29 09:14:37
 */
/*
 * Black_PWM.h
 *
 *  Created on: 2020Äê10ÔÂ8ÈÕ
 *      Author: PC
 */

#ifndef PSK_MOTOR_H_
#define PSK_MOTOR_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
extern int encode_lb, encode_rb, encode_lf, encode_rf;
extern float basic_speed;
extern int32_t encode_100ms_total;
extern int64_t encode_total;

void go_speed(float speed, float target_angle);
void servo_set(float target_angle);
void set_speed_back(float left_speed, float right_speed);
void set_speed_front(float l_speed, float r_speed);
void pid_set(float l_speed, float r_speed);

float distance_calc(void);
void until_distance(float cm);
#endif
