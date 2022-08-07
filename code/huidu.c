/*
 * @Author: Ptisak
 * @Date: 2022-07-23 20:02:44
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-30 00:16:15
 * @Version: Do not edit
 */
#include "huidu.h"
float huidu_error;
float t_a = 50;
float t_b = 30;
float t_c = 10;

// float t_a = 120; // 90-120;
// float t_b = 80;
// float t_c = 40;
float turn[] = {0, 0, 0, 0, 0, 0};
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(a) ((a) > 0 ? (a) : -(a))
huifdu_flags_t huifdu_flags;

void huidu_io_init(void)
{
    // GPIO_INIT_IN(10, 0);
    // GPIO_INIT_IN(10, 1);
    // GPIO_INIT_IN(10, 2);
    // GPIO_INIT_IN(10, 3);
    // GPIO_INIT_IN(10, 4);
    // GPIO_INIT_IN(10, 5);
    P10DIR &= ~0x3F; // 10.0    -10.5
    P10REN |= 0x3F;  // 10.0
    P8DIR &= ~0xE0;  // 8.5   -p8.7
    P8REN |= 0xE0;   //
    P10OUT = 0;
}
int sensor_read(int8_t i)
{
    return !!GPIO_READ(10, i);
}
/**
 * @brief :
 * @note  :
 * @param {float} speed
 * @param {int} trace_mode 0 默认 , 1 左优先, 2 右优先
 */
void huidu_get_error(void)
{
    static float last_error = 0;
    static float last_speed = -1;
    int i = 0;
    float error = 0;
    int sum = 0;
    // turn[0] = -t_a;
    // turn[1] = -t_b;
    // turn[2] = -t_c;
    // turn[3] = t_c;
    // turn[4] = t_b;
    // turn[5] = t_a;
    if (last_speed != basic_speed)
    {
        last_speed = basic_speed;
        t_a = map(basic_speed, 30.f, 110, 50.f, 130);
        t_b = map(basic_speed, 30.f, 110, 30.f, 85);
        t_c = map(basic_speed, 30.f, 110, 10.f, 45);
        turn[0] = -t_a;
        turn[1] = -t_b;
        turn[2] = -t_c;
        turn[3] = t_c;
        turn[4] = t_b;
        turn[5] = t_a;
    }
    // for (i = 0; i < 6; i++)
    // {
    //     OLED_ShowNum(16 * i, 3, sensor_read(i), 1, 8);
    //     // OLED_ShowNum(16 * i, 3, GPIO_READ(10, i), 3, 8);
    // }

    if (huifdu_flags.trace_mode == NONE)
    {
        huidu_error = 0;
    }
    else
    {
        if (huifdu_flags.trace_mode == RIGHT_FIRST)
        {
            for (i = 5; i >= 0; i--)
            {
                if (sensor_read(i))
                {
                    if (abs(error) < abs(turn[i]))
                        error = turn[i];
                    sum++;
                }
                else if (sum)
                    break;
            }
        }
        else
        {
            for (i = 0; i < 6; i++)
            {
                if (sensor_read(i))
                {
                    if (abs(error) < abs(turn[i]))
                        error = turn[i];
                    sum++;
                }
                else if (sum && huifdu_flags.trace_mode == LEFT_FIRST)
                    break;
            }
        }
        sum = 0;
        for (i = 0; i < 6; i++)
        {
            if (sensor_read(i))
            {
                sum++;
            }
        }

        if (sum == 0)
        {
            if (abs(last_error) > turn[4])
                error = last_error;
        }
        if (sum > 2)
        {
            error = 0;
            // 0 1 2   3 4 5
            if (sensor_read(2) && sensor_read(3) && (sensor_read(1) || sensor_read(4)))
            {
                // buzzer_bi_flag = 1;
                huifdu_flags.is_end = 1;
            }
        }
        last_error = error;
        // if (error > 0)
        // {
        //     return max(speed * error / 100.f, error)
        // }
        // return error;
        huidu_error = error + damping;
    }
}

void clear_end_flag(void)
{
    huifdu_flags.is_end = 0;
}