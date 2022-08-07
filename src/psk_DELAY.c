/*
 * @Author: Ptisak
 * @Date: 2022-07-20 14:32:48
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-27 15:46:57
 * @Version: Do not edit
 */
/* DriverLib Includes */
#include <psk_DELAY.h>
void delay_us(uint32_t x)
{
  for (uint32_t i = 0; i < (x); i++)
    for (uint32_t j = 8; j > 0; j--)
      ;
}
void delay_ms(uint32_t x)
{
  for (uint32_t i = 0; i < (x); i++)
    for (uint32_t j = 8026; j > 0; j--)
      ;
}
void sys_init(void)
{
  static volatile uint32_t aclk, mclk, smclk, hsmclk, bclk;
  /* Stop Watchdog  */
  MAP_WDT_A_holdTimer();

  /* Setting DCO to 48MHz (upping Vcore) */
  FlashCtl_setWaitState(FLASH_BANK0, 1);
  FlashCtl_setWaitState(FLASH_BANK1, 1);
  MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
  CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

  // https://blog.csdn.net/qq_43694114/article/details/108562300

  //  MAP_CS_initClockSignal(CS_MCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_1);
  /* Initializing the clock source as follows:
   *      MCLK = MODOSC/4 = 6MHz
   *      ACLK = REFO/1 = 32kHz
   *      HSMCLK = DCO/2 = 1.5Mhz
   *      SMCLK = DCO/4 = 750kHz
   *      BCLK  = REFO = 32kHz
   */
  // MAP_CS_initClockSignal(CS_MCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_4);
  // MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
  MAP_CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);
  MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
  // MAP_CS_initClockSignal(CS_ACLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_128);
  // MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
  // MAP_CS_initClockSignal(CS_SMCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_2);
  // MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

  /*
   *  Getting all of the frequency values of the CLK sources using the
   * convenience functions */
  aclk = CS_getACLK();
  mclk = CS_getMCLK();
  smclk = CS_getSMCLK();
  hsmclk = CS_getHSMCLK();
  bclk = CS_getBCLK();

  // MAP_SysTick_enableModule();
  // MAP_SysTick_setPeriod(48000);
  // MAP_SysTick_enableInterrupt();

  /* Enabling the FPU for floating point operation */
  MAP_FPU_enableModule();
  MAP_FPU_enableLazyStacking();
}
void no_used_initial(void)
{
  return;
  //--------------------------------------------------AHT10--------------------------------------------------
  // AHT10_Init(); // SDA 1.6, SCL 1.7
  //--------------------------------------------------MPU6050--------------------------------------------------
  // zf_mpu6050_init(); // SDA 3.6, SCL 3.7
  //--------------------------------------------------MPU6050 posget--------------------------------------------------
  // gyroOffsetInit();
  //--------------------------------------------------gimbals云台--------------------------------------------------
  // gimbals_set(0, 0);
  // delay_ms(1000);
  // gimbals_set(-135, 90);
  // delay_ms(1000);
  // gimbals_set(135, -90);
  // delay_ms(1000);
  // gimbals_set(0, 0);
  /*
  float deg, x, y, aafs, u3ih;
  while (1)
  {
      delay_ms(2);
      deg += 0.1;
      if (deg > 6.28)
      {
          deg -= 6.28;
      }
      x = cos(deg);
      y = sin(deg);
      aafs = asin(x) * 56 - 90;
      u3ih = acos(y) * 56 - 90;
      send_four_data(0xf1, aafs + 100, u3ih + 100, 0, 0);
      send_four_data(0xf1, x + 100, y + 100, 0, 0);
      // gimbals_set(aafs, u3ih);
      gimbals_set(x * 12, y * 12 + 8);
  }
  while (1)
  {
      delay_ms(50);
      oled_printf_int32(0, 3, eulerAngle.pitch, 10);
      oled_printf_int32(0, 4, eulerAngle.roll, 10);
      oled_printf_int32(0, 5, eulerAngle.yaw, 10);

      send_four_data(0xf1, eulerAngle.pitch, eulerAngle.roll, eulerAngle.yaw, 0);

      gimbals_set(eulerAngle.yaw, -eulerAngle.pitch);
  }
  while (1)
  {
      delay_ms(5);
      gimbals_set(humi - 90, humi - 90);

      humi += temp;
      if (humi > 180)
      {
          // humi = 0;
          temp = -0.1;
          // GPIO_TOGGLE(1, 0);
          // delay_ms(1000);
      }
      if (humi < 0)
      {
          // humi = 0;
          temp = 0.1;
          // GPIO_TOGGLE(1, 0);
          // delay_ms(1000);
      }
  }
  while (1)
  {
      ii += 300;
      if (ii > TA0CCR0)
      {
          ii = 0;
          // GPIO_TOGGLE(1, 0);
      }
      // delay_ms(80);

      // get_gyro();
      // get_accdata();
      // oled_printf_int32(0, 3, eulerAngle.pitch, 10);
      // oled_printf_int32(0, 4, eulerAngle.roll, 10);
      // oled_printf_int32(0, 5, eulerAngle.yaw, 10);

      // send_four_data(0xf1, eulerAngle.pitch, eulerAngle.roll, eulerAngle.yaw, 0);
      // send_four_data(0xf1, mpu_acc_x, mpu_acc_y, mpu_acc_z, 0);

      // PWM_CHANGE_CCR(0, 2, ii);

      AHT10_Read_Humi_Temp(&humi, &temp);
      send_four_data(0xf2, humi * 100, temp * 100, 0, 0);

      oled_printf_int32(0, 7, temp, 6);
      oled_printf_int32(64, 7, humi, 6);
      OLED_ShowString(40, 7, "C", 8);
      OLED_ShowString(100, 7, "%", 8);
  }
  */
}
// //初始化延迟函数
// // SYSTICK的时钟固定为HCLK时钟的1/8
// // SYSCLK:系统时钟
// void delay_init(uint8_t SYSCLK)
// {
//   fac_us = SYSCLK;
//   fac_ms = (uint16_t)fac_us * 1000; // ms是us的1000倍
// }
// //延时nms
// //注意nms的范围
// // SysTick->LOAD为24位寄存器,所以,最大延时为:
// // nms<=0xffffff*8*1000/SYSCLK
// // SYSCLK单位为Hz,nms单位为ms
// //对72M条件下,nms<=1864
// void delay_ms(uint16_t nms)
// {
//   uint32_t temp;
//   SysTick->LOAD = (uint32_t)nms * fac_ms;                               //时间加载(SysTick->LOAD为24bit)
//   SysTick->VAL = 0x00;                                                  //清空计数器
//   SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk; //开始倒数
//   do
//   {
//     temp = SysTick->CTRL;
//   } while (temp & 0x01 && !(temp & (1 << 16))); //等待时间到达
//   SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk; //关闭计数器
//   SysTick->VAL = 0X00;                          //清空计数器
// }
// //延时nus
// // nus为要延时的us数.
// void delay_us(uint32_t nus)
// {
//   uint32_t temp;
//   SysTick->LOAD = nus * fac_us;                                         //时间加载
//   SysTick->VAL = 0x00;                                                  //清空计数器
//   SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk; //开始倒数
//   do
//   {
//     temp = SysTick->CTRL;
//   } while (temp & 0x01 && !(temp & (1 << 16))); //等待时间到达
//   SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk; //关闭计数器
//   SysTick->VAL = 0X00;                          //清空计数器
// }

uint32_t sysTickCount;
/*
 * SysTick interrupt handler. This handler toggles RGB LED on/off.
 */
void SysTick_Handler(void)
{
  sysTickCount++;
  // MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
// void delay_ms(uint32_t x)
// {
//   // sysTickCount = 0;
//   for (sysTickCount = 0; sysTickCount < x;)
//     ;
// }
