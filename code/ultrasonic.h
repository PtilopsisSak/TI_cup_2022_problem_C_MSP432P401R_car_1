/*
 * @Author: Ptisak
 * @Date: 2022-07-27 21:42:41
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-29 23:42:48
 * @Version: Do not edit
 */
#ifndef MY_ULTRASONIC_H
#define MY_ULTRASONIC_H

#include "psk_ALL.h"
extern float ultrasonic_distance;

#define SPLIT_ULTRASONIC_UART EUSCI_A2_BASE //有来接收串口
#define SPLIT_ULTRASONIC_BAUD 115200
#define SPLIT_ULTRASONIC_RX GPIO_PORT_P3, GPIO_PIN3 //对应串口tx
#define SPLIT_ULTRASONIC_TX GPIO_PORT_P3, GPIO_PIN2 //对应串口rx

#define SEND_PORT_PIN GPIO_PORT_P6, GPIO_PIN0 //有去EN端，拉高即可

void uart_handle(void);
void send_init(void);

#endif