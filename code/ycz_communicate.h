/*
 * @Author: Ptisak
 * @Date: 2022-07-27 20:25:35
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-31 10:26:19
 * @Version: Do not edit
 */
#ifndef __ycz_com_h
#define __ycz_com_h

#include "psk_ALL.h"
extern float others_speed;

void speed_post(uint8 speed_cm);
void task_and_speed_post(uint8 task_number, uint8 speed);
void task_and_speed_post_A0(uint8 task_number, uint8 speed);
void task_stop_A0(void);

void task_post(uint8 pos);
void communicate_handler(unsigned char ucData);
void task_stop(void);
#endif
