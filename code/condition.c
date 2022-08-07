/*
 * @Author: Ptisak
 * @Date: 2022-07-31 10:22:56
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-31 12:18:56
 * @Version: Do not edit
 */
#include "condition.h"
//    Laser_detect_Scan();
//    Laser_detect

u8 condition_str[][7] = {
	"null  ",
	"L 45  ",
	"L 90  ",
	"L 135 ",
	"R 45  ",
	"R 90  ",
	"R 135 ",
	"UP    ",
	"DOWN  ",
	"FLAT  ",
	"LINE  ",
	"N up  ",
	"N down",
	"N flat",
	"seesaw",
	"diff Y",
	"obst  ",
	"LEFT  ",
	"RIGHT ",
	"COLLIS",
	"N fla2"};

int now_condition = NO_condition;
// Gyroscope_detect();
// extern float angle_x;//主俯仰角，向上为正，向下为负
// extern float angle_y;//主旋转角，逆时针为正，顺时针为负
// extern float angle_z;//测试角
void wait_for_condition(int c)
{
	static int t;
	static float angle;
	now_condition = c;
	if (now_condition == LEFT_HAVE_LINE)
	{
		while (!sensor_read(0))
		{
			delay_us(100);
		}
	}
	else if (now_condition == RIGHT_HAVE_LINE)
	{
		while (!sensor_read(5))
		{
			delay_us(100);
		}
	}
	else if (now_condition == STOP_LINE)
	{
		huifdu_flags.is_end = 0;
		//		while (!huifdu_flags.is_end)
		//		{
		//			delay_ms(5);
		//		}
		while (1)
		{
			delay_us(100);
			if (sensor_read(2) && sensor_read(3) && (sensor_read(1) || sensor_read(4)))
			{
				huifdu_flags.is_end = 1;
				break;
			}
		}
	}

	now_condition = NO_condition;
}
