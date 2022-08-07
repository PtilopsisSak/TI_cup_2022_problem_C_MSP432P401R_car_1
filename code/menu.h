/*
 * @Author: ksn
 * @Date: 2022-07-27 15:34:35
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-31 00:41:39
 */
#ifndef _MENU_H_
#define _MENU_H_
#include "psk_ALL.h"
void KEY_INIT(void);
int KEY_Scan(void);
extern uint8_t menu_speed_set;

void UI_MainMenu(void);
void CTRL_MainMenu(void);
void UI_Mission3(void);
void CTRL_Mission(void f(void));

#endif