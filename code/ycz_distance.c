/*
 * @Author: Ptisak
 * @Date: 2022-07-27 20:25:35
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-29 23:44:36
 * @Version: Do not edit
 */
#include "ycz_distance.h"

//以下变量依赖其他文件获得，分别是
//本辆车的四个轮编码器值、另一辆车通过同样方法计算的均速、超声波的距离
//单位是 1  、  cm/100ms     、    cm
extern float others_speed;
extern float ultrasonic_distance; // cm

//需要定义两个个变量，下面两个函数返回值幅值
//两车的距离值、本车的速度
float car_distance;
float my_speed;

//计算本辆车的速度，单位cm/100ms
float calc_my_speed(void)
{
	//车跑1m脉冲数 1326 1337 1381 1363
	float my_speed_;
	float k = 0.184945441094877f * 548 / 688.f;

	my_speed_ = (encode_100ms_total)*k;

	encode_100ms_total = 0;
	my_speed = my_speed_;

	return my_speed_;
}

//距离控制100ms控制一次应该够了
//输入是超声波测的距离（cm）、自己车的速度、另一辆车速度
//输出是计算的两车距离
float calc_distance(float ultrasonic_distance, float my_speed, float others_speed)
{
	//两个置信系数
	float k_encoder = 0;
	float k_sonic = 1;
	float car_distance;
	float encoder_sampling_period = 0.1; // 秒
	float encoder_distance = 0;

	static float last_ultrasonic_distance;
	static float last_car_distance;

	if (last_ultrasonic_distance == 0)
		last_ultrasonic_distance = 20.0;
	if (last_car_distance == 0)
		last_car_distance = 20.0;

	//先将明显错误的超声波数据滤除，按照上次的正常值处理
	if ((ultrasonic_distance - last_ultrasonic_distance) > 100)
		ultrasonic_distance = last_ultrasonic_distance;

	encoder_distance = last_car_distance + (my_speed - others_speed) * encoder_sampling_period;

	car_distance = k_sonic * ultrasonic_distance + k_encoder * encoder_distance;

	last_ultrasonic_distance = ultrasonic_distance;

	return car_distance;
}

float disLoop_kp = 0;
float disLoop_ki = 0;
float disLoop_kd = 0;
float target_dis = 38.0;

float disLoop_speed = 0;
float calc_disLoop_speed(float dis)
{

	float speed_output = 0;
	float dis_error = 0;
	static float last_err = 0;

	dis_error = dis - target_dis;
	speed_output = disLoop_kp * dis_error + disLoop_kd * (dis_error - last_err);

	last_err = dis_error;

	if (speed_output > 25)
		speed_output = 25.0;
	if (speed_output < -25)
		speed_output = -25.0;

	return speed_output;
}
