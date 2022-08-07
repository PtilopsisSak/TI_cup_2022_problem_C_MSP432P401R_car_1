/*
 * @Author: Ptisak
 * @Date: 2022-07-20 14:32:48
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-22 20:08:57
 * @Version: Do not edit
 */
#ifndef PSK_INTERRUPT_H_
#define PSK_INTERRUPT_H_
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
void GPIO_INIT_IT(uint8_t port, uint8_t pin);
void TA3_IT_INIT(void);

#endif
