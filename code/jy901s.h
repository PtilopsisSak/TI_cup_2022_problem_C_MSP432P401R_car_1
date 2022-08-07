/*
 * @Author: Ptisak
 * @Date: 2022-07-27 20:25:34
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-27 20:29:42
 * @Version: Do not edit
 */
#ifndef __jjjjjj_h
#define __jjjjjj_h

#include "string.h"
#include "psk_ALL.h"

struct SAcc
{
	short a[3];
	short T;
};
struct SGyro
{
	short w[3];
	short T;
};
struct SAngle
{
	short Angle[3];
	short T;
};

extern float damping;
extern float diansai_z_kp;
void jy901s_handler(unsigned char ucData);
void get_damping(void);

#endif
