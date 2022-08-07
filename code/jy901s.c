/*
 * @Author: Ptisak
 * @Date: 2022-07-27 20:25:34
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-31 01:07:03
 * @Version: Do not edit
 */
#include "jy901s.h"

struct SAcc stcAcc;
struct SGyro stcGyro;
struct SAngle stcAngle;

float damping = 0;
float diansai_z_kp = 0.01;

void jy901s_handler(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;
	ucRxBuffer[ucRxCnt++] = ucData; //将收到的数据存入缓冲区中
	if (ucRxBuffer[0] != 0x55)		//数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt = 0;
		return;
	}
	if (ucRxCnt < 11)
	{
		return;
	} //数据不满11个，则返回
	else
	{
		switch (ucRxBuffer[1]) //判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
		// memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
		case 0x51:
			memcpy(&stcAcc, &ucRxBuffer[2], 8);
			break;
		case 0x52:
			memcpy(&stcGyro, &ucRxBuffer[2], 8);
			// diansai_z_kp = map(basic_speed, -200, 200, -1, 1);
			damping = diansai_z_kp * stcGyro.w[2];
			break;
		//仅需要Z轴角度数据
		case 0x53:
			memcpy(&stcAngle, &ucRxBuffer[2], 8);
			break;
		}
		ucRxCnt = 0; //清空缓存区
	}
}

void get_damping(void)
{

	//用陀螺仪的z周角加速度当控制器的微分环节抑制振荡
	// printf("%d \r\n",(int)(stcAngle.Angle[2] / 32768 * 180));
	//	printf("%d \r\n",stcGyro.w[2]);
	//	printf("%d \r\n",stcAcc.a[2]);
	//	damping = diansai_z_kp * (*(float*)(&(stcGyro.w[2]))) / 32768 * 2000;
	// damping = (float)stcGyro.w[2]/32768*2000;
}
