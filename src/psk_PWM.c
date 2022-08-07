/*
 * @Author: Ptisak
 * @Date: 2022-06-15 11:36:20
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-27 15:33:17
 * @Version: Do not edit
 */
#include "psk_PWM.h"

void TA0_4PWM_INIT(void)
{
    P2DIR |= 0xf0;
    P2SEL0 |= 0xf0;
    TA0CCR0 = 4800;

    TA0CCTL1 = OUTMOD_7;
    TA0CCTL2 = OUTMOD_7;
    TA0CCTL3 = OUTMOD_7;
    TA0CCTL4 = OUTMOD_7;
    TA0CTL = TASSEL_2 + MC_1;

    TA0CCR0 = 4800;             // change period
    TA0CCR1 = 2400;             // change pulse P2.4    ch1, TA0.1
    TA0CCR2 = 4287;             // change pulse P2.5
    TA0CCR3 = 1527;             // change pulse P2.6
    TA0CCR4 = 787;              // change pulse P2.7
    PWM_CHANGE_CCR(0, 0, 9600); // 系统主时钟   sMCLK  48MHz   -> 5Khz
}

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig pwmConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_20,
        32000,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        3200};

void TA1_4PWM_INIT(void)
{
    // MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig);

    P7DIR |= 0xf0;
    P7SEL0 |= 0xf0;

    TA1CCR0 = 48000;

    TA1CCTL1 = OUTMOD_7;
    TA1CCTL2 = OUTMOD_7;
    TA1CCTL3 = OUTMOD_7;
    TA1CCTL4 = OUTMOD_7;
    // TA1CTL |= TIMER_A_CTL_ID__8;
    // TA1EX0 = (TIMER_A_CLOCKSOURCE_DIVIDER_64 / 8 - 1);
    TA1CTL = TASSEL_2 + MC_1;
    // TA1CTL |= TIMER_A_CLOCKSOURCE_SMCLK + TIMER_A_DO_CLEAR;

    TA1CCR0 = 48000;            // change period
    TA1CCR1 = 160;              // change pulse  P7.7
    TA1CCR2 = 42807;            // change pulse  P7.6
    TA1CCR3 = 15207;            // change pulse  P7.5
    TA1CCR4 = 7870;             // change pulse  P7.4
    PWM_CHANGE_CCR(1, 0, 9600); // 系统主时钟   sMCLK  48MHz   -> 5Khz
}
void TA2_2PWM_INIT(void)
{
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);
    P5DIR |= 0xc0;
    P5SEL0 |= 0xc0;
    TA2CCR0 = 4800;

    TA2CCTL1 = OUTMOD_7;
    TA2CCTL2 = OUTMOD_7;
    // TA0CCTL3 = OUTMOD_7;
    // TA0CCTL4 = OUTMOD_7;
    // TA2CTL = TASSEL_2 + MC_1;

    TA2CCR0 = 4800; // change period
    TA2CCR1 = 2400; // change pulse P5.6    ch1, TA0.1
    TA2CCR2 = 4287; // change pulse P5.7
    // TA0CCR3 = 1527; // change pulse P2.6
    // TA0CCR4 = 787;  // change pulse P2.7
    PWM_CHANGE_CCR(2, 0, 48200); // 系统主时钟 sMCLK div_20  2.4MHz  -> 50hz // 云台
}
