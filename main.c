/*
 * @Author: Ptisak
 * @Date: 2022-07-19 22:17:28
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-29 14:33:33
 * @Version: Do not edit
 */
/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "psk_ALL.h"

// uint32_t ii;
// float humi, temp = 0.1;
// unsigned char str[100];
int main(void)
{
  //--------------------------------------------------clock--------------------------------------------------
  sys_init();
  no_used_initial();
  //--------------------------------------------------ultrasonic--------------------------------------------------
  GPIO_INIT_OU(6, 0);
  GPIO_SET(6, 0, 1);
  //--------------------------------------------------io beep--------------------------------------------------
  GPIO_INIT_OU(6, 4);
  BEEP_OFF;
  buzzer_bi_flag = 1;
  //--------------------------------------------------io led--------------------------------------------------
  GPIO_INIT_OU(1, 0);
  GPIO_INIT_OU(5, 5);
  GPIO_INIT_OU(3, 5);
  LED_OFF;
  LED1_OFF;
  led_blink_flag = 0;
  GPIO_SET(1, 0, 0);
  //--------------------------------------------------io menu--------------------------------------------------
  GPIO_INIT_IN(1, 1);
  GPIO_INIT_IN(1, 4);
  //--------------------------------------------------encode--------------------------------------------------
  basic_speed = 0;
  GPIO_INIT_IN_NO_UP(4, 1);
  GPIO_INIT_IN_NO_UP(4, 3);
  GPIO_INIT_IN_NO_UP(4, 5);
  GPIO_INIT_IN_NO_UP(4, 7);
  GPIO_INIT_IT(4, 0); //  P4.1 和 (4.0左前 B 黄, 绿线接LED6侧)
  GPIO_INIT_IT(4, 2); //  P4.3 和 (4.2右前 A 白  绿线接LED3侧)
  GPIO_INIT_IT(4, 4); //  P4.5 和 (4.4左后 B                )
  GPIO_INIT_IT(4, 6); //  P4.7 和 (4.6右后 A                )
  // GPIO_INIT_IT(4, 1);
  // GPIO_INIT_IT(4, 3);
  // GPIO_INIT_IT(4, 5);
  // GPIO_INIT_IT(4, 7);
  //--------------------------------------------------pwm--------------------------------------------------
  TA0_4PWM_INIT(); // 后轮 wgh的驱动 P2.4 ~1 P2.5 ~2 P2.6 ~3 P2.7 ~4
  TA1_4PWM_INIT(); // 前轮 mst的驱动 P7.4 ~1 P7.5 ~2 P7.6 ~3 P7.7 ~4
  // TA2_2PWM_INIT(); //50hz的
  PWM_CHANGE_CCR(0, 1, 4800);
  //--------------------------------------------------motor--------------------------------------------------
  pid_set(0, 0); // 10ms 四闭环
  PID_struct_init(&pid_rb, POSITION_PID, 100, 100, 5, 0.5, 1.5);
  PID_struct_init(&pid_lb, POSITION_PID, 100, 100, 5, 0.5, 1.5);
  PID_struct_init(&pid_rf, POSITION_PID, 100, 100, 5, 0.5, 1.5);
  PID_struct_init(&pid_lf, POSITION_PID, 100, 100, 5, 0.5, 1.5);
  PID_struct_init(&pid_speed, POSITION_PID, 300, 300, 1, 0.01, 0.2);
  //--------------------------------------------------huidu--------------------------------------------------
  huidu_io_init(); // 10.0    -10.5  8.5   -p8.7
  //--------------------------------------------------usart--------------------------------------------------
  USART0_DEBUG_INIT();
  USART1_INIT(); // RX 2.2, TX 2.3 115200 // 无线
  USART2_INIT(); // RX 3.2, TX 3.3 115200 // 有来有去
  USART3_INIT(); // RX 9.6, TX 9.7 9600 // 九轴
  extern void EUSCIA1_IRQHandler(void);
  printf("this is uart printf test, %f, %d, %c, %s, %u, %o, %x, %p, %%\r\n",
         1.23, 67, 'a', "string", 233, 066, 0xf1, 0x40000406); // 浮点数无效, 只能输出整数部分
  //--------------------------------------------------oled--------------------------------------------------
  OLED_Init();                              // OLED初始化 SCL p6.7 SDA p6.6
  OLED_Clear();                             // OLED清屏
  OLED_ShowString(29, 0, "TI cup", 8);      //按照x，y坐标来显示字符串，最后一位是字体大小
  OLED_ShowString(29, 1, "MSP432P401R", 8); //按照x，y坐标来显示字符串，最后一位是字体大小
  // zf_sprintf(str, "%d, %s", 66, "zf");
  // OLED_ShowString(0, 2, str, 8); //按照x，y坐标来显示字符串，最后一位是字体大小
  //--------------------------------------------------ta it--------------------------------------------------
  TA3_IT_INIT();
  PWM_CHANGE_CCR(3, 0, 10000); // MCLK 48分频 1MHz -> 100Hz, 10ms
  extern void TA3_0_IRQHandler(void);
  //--------------------------------------------------main--------------------------------------------------
  led_blink_flag = 1000;
  CTRL_MainMenu();
  //--------------------------------------------------never reached here--------------------------------------------------
  led_blink_flag = 3001;
  //外圈
  while (GPIO_READ(1, 1))
  {
  }
  led_blink_flag = 0;

  basic_speed = 30;
  buzzer_bi_flag = 1;

  while (1)
  {
    huifdu_flags.trace_mode = RIGHT_FIRST;
    if (huifdu_flags.is_end)
    {
      time_count_flag = !time_count_flag;
      if (time_count_flag == 1)
      {
        delay_ms(9000);
        huifdu_flags.is_end = 0;
      }
      else
      {
        basic_speed = 0;
        buzzer_bi_flag = 1;

        led_blink_flag = 71;
        while (1)
          ;
      }
    }
  }
}
