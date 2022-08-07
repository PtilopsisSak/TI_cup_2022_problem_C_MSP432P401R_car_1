/*
 * @Author: Ptisak
 * @Date: 2022-06-15 11:36:20
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-21 14:21:09
 * @Version: Do not edit
 */

#ifndef PSK_OLED_H_
#define PSK_OLED_H_
#include "psk_ALL.h"

#include "psk_OLED_type.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

#define OLED_SCLK_Clr() P6OUT &= ~BIT7 // CLK
#define OLED_SCLK_Set() P6OUT |= BIT7

#define OLED_SDIN_Clr() P6OUT &= ~BIT6 // DIN
#define OLED_SDIN_Set() P6OUT |= BIT6

#define OLED_CMD 0
#define OLED_DATA 1

// Functions to control OLED are as follows
void OLED_WR_Byte(unsigned dat, unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size);
void OLED_ShowVI(u8 x, u8 y, u32 num, u8 size);
void OLED_ShowString(u8 x, u8 y, u8 *p, u8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void OLED_IIC_Start(void);
void OLED_IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack(void);

void oled_p6x8str(uint8 x, uint8 y, uint8 ch[]);
void oled_uint16(uint8 x, uint8 y, uint16 num);
void oled_int16(uint8 x, uint8 y, int16 num);
void oled_printf_int32(uint16 x, uint16 y, int32 dat, uint8 num);
void oled_printf_float(uint16 x, uint16 y, double dat, uint8 num, uint8 pointnum);

#endif
