/*
 * @Author: ksn
 * @Date: 2020-10-08 15:27:43
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-26 22:48:58
 */

#ifndef PSK_PWM_H_
#define PSK_PWM_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief :
 * @note  :
 *  a:TAx 0 / 1
 *  x:CCRx 0/1/2/3/4   0:the period of TIMER
 *  y:CCR value
 */
#define PWM_CHANGE_CCR(_0_or_1, _01234, _uint16) TA##_0_or_1##CCR##_01234## = (_uint16);

void TA0_4PWM_INIT(void);
void TA1_4PWM_INIT(void);

#endif
