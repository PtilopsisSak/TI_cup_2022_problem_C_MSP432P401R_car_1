/*
 * @Author: Ptisak
 * @Date: 2022-07-23 20:02:32
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-23 21:58:58
 * @Version: Do not edit
 */
#include "psk_ALL.h"
#ifndef PSK_SERVO
#define PSK_SERVO
void servo_set_angle(uint8_t n, float angle);
void gimbals_set(float angle1, float angle2);
#endif