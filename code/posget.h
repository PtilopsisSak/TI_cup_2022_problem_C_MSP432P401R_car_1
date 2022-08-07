/*
 * @Author: Ptisak
 * @Date: 2022-07-21 15:11:01
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-22 22:24:41
 * @Version: Do not edit
 */
/*
 * posget.h
 *
 *  Created on: 2022年4月30日
 *      Author: Breeze
 */

#ifndef CODE_POSGET_H_
#define CODE_POSGET_H_

#include "psk_ALL.h"
// struct vector3f_t
//     {
//         float x;
//         float y;
//         float z;
//     };
//

#define PI 3.1415926f
#define delta_T 0.01f // 采样周期5ms 即频率200HZ

extern struct icm_param_t icm_data;
extern struct euler_param_t eulerAngle;
extern float roll_get;

struct icm_param_t
{
    float acc_x;
    float acc_y;
    float acc_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
};
struct quater_param_t
{
    float q0; //四元数
    float q1;
    float q2;
    float q3;
};
struct euler_param_t
{
    float pitch;
    float roll;
    float yaw;
};
void gyroOffsetInit(void);
void icmGetValues(void);
void icmAHRSupdate(struct icm_param_t *icm);
float filter_first(float data);
void get_pos(void);

float myRsqrt(float num);

#endif /* CODE_POSGET_H_ */
