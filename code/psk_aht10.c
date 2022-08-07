#include "psk_aht10.h"

// #include "AHT10.H"

u8 ACK, DATA[6];

void AHT_I2C_UserConfig(void)
{
  GPIO_INIT_OU(1, 7);
  GPIO_INIT_OU(1, 6);
}
void AHT_I2C_SDA_Mode(u8 addr)
{

  if (addr)
  { // 1 OUT

    GPIO_INIT_OU(1, 6);
  }
  else
  { // 0 INPUT

    GPIO_INIT_IN(1, 6);
  }
}
void AHT_I2C_Start(void)
{

  AHT_I2C_SDA_Mode(OUT);

  SCL_High;
  SDA_High;
  // delay_us(0);//

  SDA_Low;
  // delay_us(0);//
  SCL_Low;
}
void AHT_I2C_Stop(void)
{

  AHT_I2C_SDA_Mode(OUT);

  SDA_Low;
  // delay_us(0);//
  SCL_High;
  // delay_us(0);//
  SDA_High;
  // delay_us(0);//
}
u8 AHT_I2C_Write_Ack(void)
{

  u8 TimeAck = 0;

  AHT_I2C_SDA_Mode(INPUT);

  SCL_High;
  // delay_us(0);//

  while (GPIO_READ(1, 6))
  {

    if (++TimeAck > 250)
    {

      AHT_I2C_Stop();
      return 1;
    }
  }
  SCL_Low;
  // delay_us(0);//

  return 0;
}
void AHT_I2C_Write_Byte(u8 Data)
{

  SCL_Low;
  // delay_us(0);//

  for (u8 i = 0; i < 8; i++)
  {

    AHT_I2C_SDA_Mode(OUT);

    if ((Data << i) & 0x80)
    {
      SDA_High;
    }
    else
    {
      SDA_Low;
    }

    SCL_High;
    // delay_us(0);//
    SCL_Low;
    // delay_us(0);//
  }
}
u8 AHT_I2C_Read_Data(void)
{

  u8 Data = 0;

  for (u8 i = 0; i < 8; i++)
  {

    AHT_I2C_SDA_Mode(INPUT);

    SCL_High;
    // delay_us(0);//
    Data <<= 1;

    if (GPIO_READ(1, 6))
    {

      Data |= 0x01;
    }

    SCL_Low;
    // delay_us(0);//
  }
  return Data;
}
void AHT_I2C_Sende_Ack(u8 ack)
{

  AHT_I2C_SDA_Mode(OUT);

  if (ack)
  {
    SDA_High;
  }
  else
  {
    SDA_Low;
  }

  SCL_High;
  // delay_us(0);//
  SCL_Low;
  // delay_us(0);//
}
u8 AHT10_State(void)
{
  //存在0 不存在 1

  u8 ACK;

  AHT_I2C_Start();
  AHT_I2C_Write_Byte(AHT_WRITE);
  ACK = AHT_I2C_Write_Ack();
  AHT_I2C_Stop();

  return ACK;
}
void AHT10_Init(void)
{
  //	bit3 0 1
  AHT_I2C_UserConfig();

  AHT_I2C_Start();
  AHT_I2C_Write_Byte(AHT_WRITE);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(0XE1);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(0X08);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(0X00);
  AHT_I2C_Write_Ack();
  AHT_I2C_Stop();
  delay_ms(40);

  AHT_I2C_Start();
  AHT_I2C_Write_Byte(AHT_WRITE);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(AHT10_NORMAL_CMD);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(AHT10_DATA_NOP);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(AHT10_DATA_NOP);
  AHT_I2C_Write_Ack();
  AHT_I2C_Stop();
  delay_ms(50);
}
void AHT10_Write_Reset(void)
{
  AHT_I2C_Start();
  AHT_I2C_Write_Byte(AHT_WRITE);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(0XBA);
  AHT_I2C_Write_Ack();
  AHT_I2C_Stop();
  delay_ms(20);
}
u8 AHT10_Read_Humi_Temp(float *HUMI, float *TEMP)
{
  // 0数据正常 1 数据错误
  u32 humi = 0, temp = 0;

  AHT_I2C_Start();
  AHT_I2C_Write_Byte(AHT_WRITE);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(0XAC);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(0X33);
  AHT_I2C_Write_Ack();
  AHT_I2C_Write_Byte(0X00);
  AHT_I2C_Write_Ack();
  AHT_I2C_Stop();
  delay_ms(80);

  AHT_I2C_Start();
  AHT_I2C_Write_Byte(AHT_READ);
  AHT_I2C_Write_Ack();
  ACK = AHT_I2C_Read_Data();
  AHT_I2C_Sende_Ack(0); // 0000 1000 BIT3 0

  if ((ACK & 0X08) == 0)
  {
    AHT10_Init();
  }
  if ((ACK & 0X80) == 0)
  {
    // bit7 1 0
    for (u8 i = 0; i < 5; i++)
    { // 0 1 2 3 4 5 ++i

      DATA[i] = AHT_I2C_Read_Data();

      if (i == 4)
        AHT_I2C_Sende_Ack(1);
      else
        AHT_I2C_Sende_Ack(0);
    }
    AHT_I2C_Stop();

    humi = (DATA[0] << 12) | (DATA[1] << 4) | (DATA[2] >> 4);
    temp = ((DATA[2] & 0X0F) << 16) | (DATA[3] << 8) | (DATA[4]);

    *HUMI = ((float)humi * 100.0 / 1024 / 1024 + 0.5);
    *TEMP = ((float)temp * 2000.0 / 1024 / 1024 + 0.5) / 10.0 - 50;
    return 0;
  }

  AHT_I2C_Stop();
  return 1;
}
//-----------------------------------------------------------------------------------------------------------------
#define delay delay_ms
uint8 _rawDataBuffer[6];
uint8_t AHT10_readRawData()
{
  /* send measurment command */
  simiic_start();
  send_ch(AHT10_ADDRESS_0X38);

  send_ch(AHT10_START_MEASURMENT_CMD);
  send_ch(AHT10_DATA_MEASURMENT_CMD);
  send_ch(AHT10_DATA_NOP);
  simiic_stop();

  // if (AHT10_getCalibrationBit() != 0x01)
  // return AHT10_ERROR; // error handler, calibration coefficient turned off
  // if (AHT10_getBusyBit(AHT10_USE_READ_DATA) != 0x00)
  delay(AHT10_MEASURMENT_DELAY); // measurement delay

  /* read 6-bytes from sensor */
  simiic_read_regs(AHT10_ADDRESS_0X38, 0x00, _rawDataBuffer, 6, SCCB);

  return true;
}

uint8_t AHT10_readStatusByte()
{
  simiic_start();
  send_ch((AHT10_ADDRESS_0X38 << 1) | 0x01); //发送器件地址加读位
  _rawDataBuffer[0] = read_ch(0);            //读取数据
  simiic_stop();
}
bool AHT10_enableFactoryCalCoeff()
{
  simiic_start();
  send_ch((AHT10_ADDRESS_0X38 << 1) | 0x01); //发送器件地址加读位

  /* load factory calibration coeff */
  send_ch(AHT10_INIT_CMD);
  send_ch(AHT10_INIT_CAL_ENABLE);
  send_ch(AHT10_DATA_NOP);

  simiic_stop();

  delay(AHT10_CMD_DELAY);

  /*check calibration enable */
}

bool AHT10_setNormalMode(void)
{
  simiic_start();
  send_ch((AHT10_ADDRESS_0X38 << 1) | 0x00);

  send_ch(AHT10_NORMAL_CMD);
  send_ch(AHT10_DATA_NOP);
  send_ch(AHT10_DATA_NOP);
  simiic_stop();

  delay(AHT10_CMD_DELAY);

  return true;
}
bool AHT10_softReset(void)
{
  simiic_start();
  send_ch((AHT10_ADDRESS_0X38 << 1) | 0x00);

  send_ch(AHT10_SOFT_RESET_CMD);
  simiic_stop();

  delay(AHT10_SOFT_RESET_CMD);

  AHT10_setNormalMode(); // reinitialize sensor registers after reset

  return AHT10_enableFactoryCalCoeff(); // reinitialize sensor registers after reset
}
/**************************************************************************/
/*
    readTemperature()

    Read temperature, 掳C

    NOTE:
    - temperature range      -40掳C..+80掳C
    - temperature resolution 0.01掳C
    - temperature accuracy   卤0.3掳C
*/
/**************************************************************************/
float AHT10_readTemperature()
{
  if (AHT10_readRawData() == AHT10_ERROR)
    return AHT10_ERROR; // force to read data to _rawDataBuffer & error handler

  if (_rawDataBuffer[0] == AHT10_ERROR)
    return AHT10_ERROR; // error handler, collision on I2C bus

  uint32_t temperature = ((uint32_t)(_rawDataBuffer[3] & 0x0F) << 16) | ((uint16_t)_rawDataBuffer[4] << 8) | _rawDataBuffer[5]; // 20-bit raw temperature data

  return (float)temperature * 0.000191 - 50;
}

/**************************************************************************/
/*
    readHumidity()

    Read relative humidity, %

    NOTE:
    - prolonged exposure for 60 hours at humidity > 80% can lead to a
      temporary drift of the signal +3%. Sensor slowly returns to the
      calibrated state at normal operating conditions.
    - relative humidity range      0%..100%
    - relative humidity resolution 0.024%
    - relative humidity accuracy   卤2%
*/
/**************************************************************************/
float AHT10_readHumidity()
{
  if (AHT10_readRawData() == AHT10_ERROR)
    return AHT10_ERROR; // force to read data to _rawDataBuffer & error handler

  if (_rawDataBuffer[0] == AHT10_ERROR)
    return AHT10_ERROR; // error handler, collision on I2C bus

  uint32_t rawData = (((uint32_t)_rawDataBuffer[1] << 16) | ((uint16_t)_rawDataBuffer[2] << 8) | (_rawDataBuffer[3])) >> 4; // 20-bit raw humidity data

  float humidity = (float)rawData * 0.000095;

  if (humidity < 0)
    return 0;
  if (humidity > 100)
    return 100;
  return humidity;
}

bool AHT10_begin(void)
{
  simiic_init();

  delay(AHT10_POWER_ON_DELAY); // wait for sensor to initialize
  AHT10_setNormalMode();       // one measurement+sleep mode

  return AHT10_enableFactoryCalCoeff(); // load factory calibration coeff
}
