/*
 * @Author: Ptisak
 * @Date: 2022-07-23 20:02:32
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-28 14:46:08
 * @Version: Do not edit
 */
#include "psk_ALL.h"
#ifndef PSK_BEEP_LED
#define PSK_BEEP_LED
extern int buzzer_bi_flag, led_blink_flag, time_count_flag;
void led_lignt_ctl(void);
void buzzer_bbbi(void);
#endif