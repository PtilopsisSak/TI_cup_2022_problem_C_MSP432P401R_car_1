/*
 * @Author: ksn
 * @Date: 2020-10-08 13:51:38
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-22 11:43:49
 */

#include "psk_OLED.h"
#include "psk_OLED_fout.h"
#include "psk_OLED_type.h"

// the storage format is as follow
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
/**********************************************
//IIC Start
**********************************************/
void OLED_IIC_Start()
{

    OLED_SCLK_Set();
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void OLED_IIC_Stop(void)
{
    OLED_SCLK_Set();
    //  OLED_SCLK_Clr();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();
}

void IIC_Wait_Ack(void)
{
    OLED_SCLK_Set();
    OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
    unsigned char m, da;
    da = IIC_Byte;
    OLED_SCLK_Clr();
    for (i = 0; i < 8; i++)
    {
        m = da;
        //  OLED_SCLK_Clr();
        m = m & 0x80;
        if (m == 0x80)
        {
            OLED_SDIN_Set();
        }
        else
            OLED_SDIN_Clr();
        da = da << 1;
        OLED_SCLK_Set();
        OLED_SCLK_Clr();
    }
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
    OLED_IIC_Start();
    Write_IIC_Byte(0x78); // Slave address,SA0=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00); // write command
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();
    OLED_IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
    OLED_IIC_Start();
    Write_IIC_Byte(0x78); // D/C#=0; R/W#=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40); // write data
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();
    OLED_IIC_Stop();
}
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
    if (cmd)
    {
        Write_IIC_Data(dat);
    }
    else
    {
        Write_IIC_Command(dat);
    }
}

void Delay_1ms(unsigned int Del_1ms)
{
    unsigned char j;
    while (Del_1ms--)
    {
        for (j = 0; j < 123; j++)
            ;
    }
}

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);
    OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}

void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);
    OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}

void OLED_Clear(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);
        OLED_WR_Byte(0x00, OLED_CMD);
        OLED_WR_Byte(0x10, OLED_CMD);
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(0, OLED_DATA);
    } //?ü??????
}
void OLED_On(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);
        OLED_WR_Byte(0x00, OLED_CMD);
        OLED_WR_Byte(0x10, OLED_CMD);
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(1, OLED_DATA);
    }
}

// x:0~127
// y:0~63
// size:16/12
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' ';
    if (x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16)
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
    }
    else
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 6; i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);
    }
}

u32 oled_pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}

void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
    }
}
void OLED_ShowVI(u8 x, u8 y, u32 num, u8 size2)
{
    OLED_ShowNum(x + size2 * 4 - 1, y, num % 10, 1, size2);
    OLED_ShowNum(x + size2 * 3 - 1, y, num / 10 % 10, 1, size2);
    OLED_ShowNum(x + size2 * 2 - 1, y, num / 100 % 10, 1, size2);
    // OLED_ShowString(x+size2*1-1,y,".",size2);
    // OLED_ShowNum(x-1,y,num/1000,1,size2);
}

void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], Char_Size);
        x += 8;
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

void OLED_Init(void)
{
    P6DIR |= BIT7 + BIT6;
    delay_ms(200);
    OLED_WR_Byte(0xAE, OLED_CMD); //--display off
    OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
    OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
    OLED_WR_Byte(0x81, OLED_CMD); // contract control
    OLED_WR_Byte(0xFF, OLED_CMD); //--128
    OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap
    OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
    OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
    OLED_WR_Byte(0x00, OLED_CMD); //

    OLED_WR_Byte(0xD5, OLED_CMD); // set osc division
    OLED_WR_Byte(0x80, OLED_CMD); //

    OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
    OLED_WR_Byte(0x05, OLED_CMD); //

    OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
    OLED_WR_Byte(0xF1, OLED_CMD); //

    OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion
    OLED_WR_Byte(0x12, OLED_CMD); //

    OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh
    OLED_WR_Byte(0x30, OLED_CMD); //

    OLED_WR_Byte(0x8D, OLED_CMD); // set charge pump enable
    OLED_WR_Byte(0x14, OLED_CMD); //

    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_hexascii(uint16 hex, int8 *Print)
{
    uint8 hexcheck;
    uint8 TEMP;
    TEMP = 6;
    Print[TEMP] = '\0';
    while (TEMP)
    {
        TEMP--;
        hexcheck = hex % 10;
        hex /= 10;
        Print[TEMP] = hexcheck + 0x30;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示字符串(6*8字体)
//  @param      x			x轴坐标设置0-127
//  @param      y           y轴坐标设置0-7
//  @param      ch[]        字符串
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_p6x8str(uint8 x, uint8 y, uint8 ch[])
{
    OLED_ShowString(x, y, ch, 8);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示无符号数(6*8字体)
//  @param      x			x轴坐标设置0-127
//  @param      y           y轴坐标设置0-7
//  @param      num         无符号数
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_uint16(uint8 x, uint8 y, uint16 num)
{
    int8 ch[7];

    oled_hexascii(num, ch);

    oled_p6x8str(x, y, &ch[1]); //显示数字  6*8字体
    // oled_p8x16str(x, y, &ch[1]);	//显示数字  8*16字体
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示有符号数(6*8字体)
//  @param      x			x轴坐标设置0-127
//  @param      y           y轴坐标设置0-7
//  @param      num         有符号数
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_int16(uint8 x, uint8 y, int16 num)
{
    int8 ch[7];
    if (num < 0)
    {
        num = -num;
        oled_p6x8str(x, y, "-");
    }
    else
        oled_p6x8str(x, y, " ");
    x += 6;

    oled_hexascii(num, ch);
    oled_p6x8str(x, y, &ch[1]); //显示数字  6*8字体
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示32位有符号(去除整数部分无效的0)
//  @param      x			x轴坐标设置0-127
//  @param      y           y轴坐标设置0-7
//  @param      dat       	需要显示的变量，数据类型uint32
//  @param      num       	需要显示的位数 最高10位  不包含正负号
//  @return     void
//  @since      v1.0
//  Sample usage:           oled_printf_int32(0,0,x,5);//x可以为int32 uint16 int16 uint8 int8类型
//  Sample usage:           负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void oled_printf_int32(uint16 x, uint16 y, int32 dat, uint8 num)
{
    int8 buff[34];
    uint32 length;

    if (10 < num)
        num = 10;

    num++;
    if (0 > dat)
        length = zf_sprintf(&buff[0], "%d", dat); //负数
    else
    {
        buff[0] = ' ';
        length = zf_sprintf(&buff[1], "%d", dat);
        length++;
    }
    while (length < num)
    {
        buff[length] = ' ';
        length++;
    }
    buff[num] = '\0';

    oled_p6x8str((uint8)x, (uint8)y, buff); //显示数字
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示浮点数(去除整数部分无效的0)
//  @param      x			x轴坐标设置0-127
//  @param      y           y轴坐标设置0-7
//  @param      dat       	需要显示的变量，数据类型float或double
//  @param      num         整数位显示长度   最高10位
//  @param      pointnum    小数位显示长度   最高6位
//  @return     void
//  @since      v1.0
//  Sample usage:           oled_printf_float(0,0,x,2,3);//显示浮点数   整数显示2位   小数显示三位
//  @note                   特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//                          可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//                          有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//                          负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void oled_printf_float(uint16 x, uint16 y, double dat, uint8 num, uint8 pointnum)
{
    uint32 length;
    int8 buff[34];
    int8 start, end, point;

    if (6 < pointnum)
        pointnum = 6;
    if (10 < num)
        num = 10;

    if (0 > dat)
        length = zf_sprintf(&buff[0], "%f", dat); //负数
    else
    {
        length = zf_sprintf(&buff[1], "%f", dat);
        length++;
    }
    point = (int8)(length - 7); //计算小数点位置
    start = point - num - 1;    //计算起始位
    end = point + pointnum + 1; //计算结束位
    while (0 > start)           //整数位不够  末尾应该填充空格
    {
        buff[end] = ' ';
        end++;
        start++;
    }

    if (0 > dat)
        buff[start] = '-';
    else
        buff[start] = ' ';

    buff[end] = '\0';

    oled_p6x8str((uint8)x, (uint8)y, buff); //显示数字
}
