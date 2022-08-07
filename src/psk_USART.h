/*
 * @Author: Ptisak
 * @Date: 2022-07-20 14:32:48
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-31 10:23:52
 * @Version: Do not edit
 */
#ifndef PSK_USART_H_
#define PSK_USART_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "SEEKFREE_PRINTF.h"
extern unsigned char printf_temp[200];

// #define printf(...) USART0_DEBUG_SEND((zf_sprintf(printf_temp, __VA_ARGS__), printf_temp));
#define printf(...)

void USART0_DEBUG_INIT(void);
void USART2_INIT(void);
void USART2_SEND(unsigned char *ptr);

void send_four_data(uint8_t frame, int16_t num1, int16_t num2, int16_t num3, int16_t num4);

void USART1_INIT(void);
void USART0_DEBUG_SEND(unsigned char *ptr);
void USART3_INIT(void);
#endif
