/*
 * @Author: Ptisak
 * @Date: 2022-07-21 16:58:24
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-21 21:34:24
 * @Version: Do not edit
 */
/***************************************************************************************************/
/*
   This is an Arduino library for Aosong ASAIR AHT10, AHT15 Digital Humidity & Temperature Sensor

   written by : enjoyneering79
   sourse code: https://github.com/enjoyneering/


   This chip uses I2C bus to communicate, specials pins are required to interface
   Board:                                    SDA                    SCL                    Level
   Uno, Mini, Pro, ATmega168, ATmega328..... A4                     A5                     5v
   Mega2560................................. 20                     21                     5v
   Due, SAM3X8E............................. 20                     21                     3.3v
   Leonardo, Micro, ATmega32U4.............. 2                      3                      5v
   Digistump, Trinket, ATtiny85............. 0/physical pin no.5    2/physical pin no.7    5v
   Blue Pill, STM32F103xxxx boards.......... PB7                    PB6                    3.3v/5v
   ESP8266 ESP-01........................... GPIO0/D5               GPIO2/D3               3.3v/5v
   NodeMCU 1.0, WeMos D1 Mini............... GPIO4/D2               GPIO5/D1               3.3v/5v
   ESP32.................................... GPIO21/D21             GPIO22/D22             3.3v

   Frameworks & Libraries:
   ATtiny  Core          - https://github.com/SpenceKonde/ATTinyCore
   ESP32   Core          - https://github.com/espressif/arduino-esp32
   ESP8266 Core          - https://github.com/esp8266/Arduino
   STM32   Core          - https://github.com/stm32duino/Arduino_Core_STM32
                         - https://github.com/rogerclarkmelbourne/Arduino_STM32

   GNU GPL license, all text above must be included in any redistribution,
   see link for details  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/
#include <psk_ALL.h>
// #define _AHT10_H_

#ifndef AHT10_h
#define AHT10_h

#ifndef _AHT10_H_
#define _AHT10_H_

// #define SDA GPIO_Pin_0
// #define SCL GPIO_Pin_1
// #define I2C_Prot GPIOF

#define SDA_High GPIO_SET(1, 6, 1)
#define SDA_Low GPIO_SET(1, 6, 0)

#define SCL_High GPIO_SET(1, 7, 1)
#define SCL_Low GPIO_SET(1, 7, 0)

#define OUT 1
#define INPUT 0

void AHT_I2C_UserConfig(void);
void AHT_I2C_Start(void);
void AHT_I2C_Stop(void);
u8 AHT_I2C_Write_Ack(void);
void AHT_I2C_Write_Byte(u8 Data);
u8 AHT_I2C_Read_Data(void);
void AHT_I2C_Sende_Ack(u8 ack);

#define AHT_ADDRESS 0X70 // 0X38
#define AHT_WRITE 0X70
#define AHT_READ 0X71 // 0X39

extern u8 ACK, DATA[6];

u8 AHT10_State(void);
void AHT10_Write_Reset(void);
u8 AHT10_Read_Humi_Temp(float *HUMI, float *TEMP);
void AHT10_Init(void);

#endif

// // aht10.h
// #define AHT10_IIC_ADDR 0x38 // AHT10 IIC地址

// #define AHT10_CALIBRATION_CMD 0xE1 //校准命令(上电后只需要发送一次)
// #define AHT10_NORMAL_CMD 0xA8      //正常工作模式
// #define AHT10_GET_DATA 0xAC        //读取数据命令

// u8 AHT10_Init(void);
// float AHT10_Read_Temperature(void);
// float AHT10_Read_Humidity(void);

#define AHT10_ADDRESS_0X38 0x38 // chip I2C address no.1 for AHT10/AHT15/AHT20, address pin connected to GND
#define AHT10_ADDRESS_0X39 0x39 // chip I2C address no.2 for AHT10 only, address pin connected to Vcc

#define AHT10_INIT_CMD 0xE1             // initialization command for AHT10/AHT15
#define AHT20_INIT_CMD 0xBE             // initialization command for AHT20
#define AHT10_START_MEASURMENT_CMD 0xAC // start measurment command
#define AHT10_NORMAL_CMD 0xA8           // normal cycle mode command, no info in datasheet!!!
#define AHT10_SOFT_RESET_CMD 0xBA       // soft reset command

#define AHT10_INIT_NORMAL_MODE 0x00 // enable normal mode
#define AHT10_INIT_CYCLE_MODE 0x20  // enable cycle mode
#define AHT10_INIT_CMD_MODE 0x40    // enable command mode
#define AHT10_INIT_CAL_ENABLE 0x08  // load factory calibration coeff

#define AHT10_DATA_MEASURMENT_CMD 0x33 // no info in datasheet!!! my guess it is DAC resolution, saw someone send 0x00 instead
#define AHT10_DATA_NOP 0x00            // no info in datasheet!!!

#define AHT10_MEASURMENT_DELAY 80 // at least 75 milliseconds
#define AHT10_POWER_ON_DELAY 40   // at least 20..40 milliseconds
#define AHT10_CMD_DELAY 350       // at least 300 milliseconds, no info in datasheet!!!
#define AHT10_SOFT_RESET_DELAY 20 // less than 20 milliseconds

#define AHT10_FORCE_READ_DATA true // force to read data
#define AHT10_USE_READ_DATA false  // force to use data from previous read
#define AHT10_ERROR 0xFF           // returns 255, if communication error is occurred

#define AHT10_SENSOR 0x00

// AHT10(uint8_t address = AHT10_ADDRESS_0X38, ASAIR_I2C_SENSOR = AHT10_SENSOR);

bool AHT10_begin(void);
uint8_t AHT10_readRawData(void);
// float readTemperature(bool readI2C = AHT10_FORCE_READ_DATA);
// float readHumidity(bool readI2C = AHT10_FORCE_READ_DATA);
bool AHT10_softReset(void);
bool AHT10_setNormalMode(void);
bool AHT10_setCycleMode(void);
float AHT10_readHumidity(void);
float AHT10_readTemperature(void);

extern uint8_t _rawDataBuffer[6];

uint8_t AHT10_readStatusByte(void);
// uint8_t getCalibrationBit(bool readI2C = AHT10_FORCE_READ_DATA);
bool AHT10_enableFactoryCalCoeff(void);
// uint8_t getBusyBit(bool readI2C = AHT10_FORCE_READ_DATA);

#endif
