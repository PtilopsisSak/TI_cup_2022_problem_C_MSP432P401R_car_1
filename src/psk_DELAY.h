/*
 * @Author: Ptisak
 * @Date: 2022-07-20 14:32:48
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-27 15:32:23
 * @Version: Do not edit
 */

#ifndef PSK_DELAY_H_
#define PSK_DELAY_H_
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

void delay_us(uint32_t x);
void delay_ms(uint32_t x);
void sys_init(void);
void no_used_initial(void);
#endif
