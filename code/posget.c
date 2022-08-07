/*
 * posget.c
 *
 *  Created on: 2022年4月30日
 *      Author: Breeze
 */

#include "posget.h"
#include "math.h"
#define icm_gyro_x mpu_gyro_x
#define icm_gyro_y mpu_gyro_y
#define icm_gyro_z mpu_gyro_z

#define icm_acc_x mpu_acc_x
#define icm_acc_y mpu_acc_y
#define icm_acc_z mpu_acc_z
struct
{
    int16 Xdata;
    int16 Ydata;
    int16 Zdata;
} GyroOffset;
void gyroOffsetInit(void)
{
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
    for (uint16 i = 0; i < 100; ++i)
    {
        get_gyro(); // 获取陀螺仪角速度
        GyroOffset.Xdata += icm_gyro_x;
        GyroOffset.Ydata += icm_gyro_y;
        GyroOffset.Zdata += icm_gyro_z;
        delay_ms(5); // 最大 1Khz
    }

    GyroOffset.Xdata /= 100;
    GyroOffset.Ydata /= 100;
    GyroOffset.Zdata /= 100;
}

struct icm_param_t icm_data;
void icmGetValues(void)
{
    float alpha = 0.3;
    get_accdata();
    get_gyro();
    //一阶低通滤波，单位g
    icm_data.acc_x = (((float)icm_acc_x) * alpha) / 4096 + icm_data.acc_x * (1 - alpha);
    icm_data.acc_y = (((float)icm_acc_y) * alpha) / 4096 + icm_data.acc_y * (1 - alpha);
    icm_data.acc_z = (((float)icm_acc_z) * alpha) / 4096 + icm_data.acc_z * (1 - alpha);

    //! 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
    icm_data.gyro_x = ((float)icm_gyro_x - GyroOffset.Xdata) * PI / 180 / 16.4f;
    icm_data.gyro_y = ((float)icm_gyro_y - GyroOffset.Ydata) * PI / 180 / 16.4f;
    icm_data.gyro_z = ((float)icm_gyro_z - GyroOffset.Zdata) * PI / 180 / 16.4f;
}

float I_ex, I_ey, I_ez;                      // 误差积分
struct quater_param_t Q_info = {1, 0, 0, 0}; // 四元数初始化
struct euler_param_t eulerAngle;             // 欧拉角
float roll_get;
// float icm_kp= 0.17;    // 加速度计的收敛速率比例增益
// float icm_ki= 0.004;   // 陀螺仪收敛速率的积分增益
// float icm_kp= 0.97;    // 加速度计的收敛速率比例增益
// float icm_ki= 0.04;   // 陀螺仪收敛速率的积分增益
float icm_kp = 0.5;    // 加速度计的收敛速率比例增益
float icm_ki = 0.0001; // 陀螺仪收敛速率的积分增益
void icmAHRSupdate(struct icm_param_t *icm)
{
    float halfT = 0.5 * delta_T; // 采样周期一半
    float vx, vy, vz;            // 当前姿态计算得来的重力在三轴上的分量
    float ex, ey, ez;            // 当前加速计测得的重力加速度在三轴上的分量与用当前姿态计算得来的重力在三轴上的分量的误差

    float q0 = Q_info.q0; //四元数
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;

    float q0q0 = q0 * q0; //先相乘，方便后续计算
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    eulerAngle.pitch = asinf(2 * q0 * q2 - 2 * q1 * q3) * 180 / PI;
    eulerAngle.roll = atan2f(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 180 / PI;
    eulerAngle.yaw = atan2f(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 180 / PI;

    roll_get = ((float)(int)((eulerAngle.roll + 0.5) * 100)) / 100.;
    // 正常静止状态为-g 反作用力。
    if (icm->acc_x * icm->acc_y * icm->acc_z == 0) // 加计处于自由落体状态时(此时g = 0)不进行姿态解算，因为会产生分母无穷大的情况
        return;

    // 对加速度数据进行归一化 得到单位加速度 (a^b -> 载体坐标系下的加速度)
    float norm = myRsqrt(icm->acc_x * icm->acc_x + icm->acc_y * icm->acc_y + icm->acc_z * icm->acc_z);
    icm->acc_x = icm->acc_x * norm;
    icm->acc_y = icm->acc_y * norm;
    icm->acc_z = icm->acc_z * norm;

    // 载体坐标系下重力在三个轴上的分量
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // g^b 与 a^b 做向量叉乘，得到陀螺仪的校正补偿向量e的系数
    ex = icm->acc_y * vz - icm->acc_z * vy;
    ey = icm->acc_z * vx - icm->acc_x * vz;
    ez = icm->acc_x * vy - icm->acc_y * vx;

    // 误差累加
    I_ex += halfT * ex;
    I_ey += halfT * ey;
    I_ez += halfT * ez;

    // 使用PI控制器消除向量积误差(陀螺仪漂移误差)
    icm->gyro_x = icm->gyro_x + icm_kp * ex + icm_ki * I_ex;
    icm->gyro_y = icm->gyro_y + icm_kp * ey + icm_ki * I_ey;
    icm->gyro_z = icm->gyro_z + icm_kp * ez + icm_ki * I_ez;

    // 一阶龙格库塔法求解四元数微分方程，其中halfT为测量周期的1/2，gx gy gz为b系陀螺仪角速度。
    q0 = q0 + (-q1 * icm->gyro_x - q2 * icm->gyro_y - q3 * icm->gyro_z) * halfT;
    q1 = q1 + (q0 * icm->gyro_x + q2 * icm->gyro_z - q3 * icm->gyro_y) * halfT;
    q2 = q2 + (q0 * icm->gyro_y - q1 * icm->gyro_z + q3 * icm->gyro_x) * halfT;
    q3 = q3 + (q0 * icm->gyro_z + q1 * icm->gyro_y - q2 * icm->gyro_x) * halfT;

    // 单位化四元数在空间旋转时不会拉伸，仅有旋转角度，下面算法类似线性代数里的正交变换
    norm = myRsqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    Q_info.q0 = q0 * norm;
    Q_info.q1 = q1 * norm;
    Q_info.q2 = q2 * norm;
    Q_info.q3 = q3 * norm; // 用全局变量记录上一次计算的四元数值
}

float myRsqrt(float num)
{
    float halfx = 0.5f * num;
    float y = num;
    long i = *(long *)&y;
    i = 0x5f375a86 - (i >> 1);

    y = *(float *)&i;
    y = y * (1.5f - (halfx * y * y));
    y = y * (1.5f - (halfx * y * y));

    return y;
}
float filter_first(float data)
{
    static float data_out;
    data_out = data_out * 0.9;
    data_out = data_out + data * 0.1;
    return data_out;
}
void get_pos(void)
{
    icmGetValues();
    icmAHRSupdate(&icm_data);
}