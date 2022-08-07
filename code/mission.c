/*
 * @Author: Ptisak
 * @Date: 2022-07-23 20:02:44
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-31 13:12:36
 * @Version: Do not edit
 */
#include "mission.h"
int32_t mission_time_count;

void Mission_1(void)
{ // 18.28s
    led_blink_flag = 0;
    huifdu_flags.trace_mode = RIGHT_FIRST;
    basic_speed = 30;

    task_and_speed_post(1, (uint8_t)basic_speed);
    time_count_flag = 1;
    until_distance(312);

    wait_for_condition(STOP_LINE);
    task_stop();

    time_count_flag = 0;
    basic_speed = 0;
    buzzer_bi_flag = 1;
    led_blink_flag = 71;
}
void Mission_2(void)
{ // 21.92s
    led_blink_flag = 0;
    huifdu_flags.trace_mode = RIGHT_FIRST;
    basic_speed = 50;

    task_and_speed_post(2, (uint8_t)basic_speed);
    time_count_flag = 1;
    until_distance(312);

    wait_for_condition(STOP_LINE);
    until_distance(312);

    wait_for_condition(STOP_LINE);

    task_stop();

    time_count_flag = 0;
    basic_speed = 0;
    buzzer_bi_flag = 1;
    led_blink_flag = 71;
}
void Mission_3(void)
{
    led_blink_flag = 0;
    huifdu_flags.trace_mode = RIGHT_FIRST;
    basic_speed = menu_speed_set * 10;
    static float temp_speed_1, temp_speed_2;
    if (basic_speed <= 45)
    {
        // basic_speed += 2;

        temp_speed_1 = basic_speed * 0.2;
        temp_speed_2 = basic_speed * 0.15;
    }
    else if (basic_speed <= 85)
    {
        basic_speed += 2;

        temp_speed_1 = basic_speed * 0.2;
        temp_speed_2 = basic_speed * 0.15;
    }
    else if (basic_speed <= 105)
    {
        basic_speed += 4;

        temp_speed_1 = basic_speed * 0.4;
        temp_speed_2 = basic_speed * 0.15;
    }
    else
    {
        temp_speed_1 = basic_speed * 0.5;
        temp_speed_2 = basic_speed * 0.20;
    }

    float basic_speed_tmp = basic_speed;

    task_and_speed_post(3, (uint8_t)basic_speed);
    time_count_flag = 1;
    until_distance(312);

    wait_for_condition(STOP_LINE);
    //---------------------------
    until_distance(10);
    huifdu_flags.trace_mode = RIGHT_FIRST;

    wait_for_condition(LEFT_HAVE_LINE);
    until_distance(40);

    // basic_speed = 0.85 * basic_speed_tmp;
    basic_speed -= temp_speed_1;
    // until_distance(40);
    GPIO_SET(6, 0, 0);

    // wait_for_condition(LEFT_HAVE_LINE);
    until_distance(110);

    disLoop_kp = 1.1;

    until_distance(10);
    basic_speed = basic_speed_tmp;

    until_distance(100);
    wait_for_condition(STOP_LINE);
    //---------------------------
    until_distance(10);
    huifdu_flags.trace_mode = LEFT_FIRST;

    wait_for_condition(RIGHT_HAVE_LINE);
    basic_speed += temp_speed_2;

    led_blink_flag = 71;
    until_distance(30);

    wait_for_condition(RIGHT_HAVE_LINE);
    until_distance(10);
    basic_speed = basic_speed_tmp;
    GPIO_SET(6, 0, 1);
    disLoop_kp = 0;

    led_blink_flag = 0;
    until_distance(100);

    wait_for_condition(STOP_LINE);
    //---------------------------
    task_stop();

    time_count_flag = 0;
    basic_speed = 0;
    buzzer_bi_flag = 1;
    led_blink_flag = 71;
}
void Mission_4(void)
{
    int time_end;
    led_blink_flag = 0;
    huifdu_flags.trace_mode = RIGHT_FIRST;
    basic_speed = menu_speed_set * 10;

    task_and_speed_post(2, (uint8_t)basic_speed);
    time_count_flag = 1;
    until_distance(312);

    wait_for_condition(STOP_LINE);
    task_stop();

    basic_speed = 0;
    time_end = mission_time_count + 501;
    while (time_end > mission_time_count)
    {
        delay_ms(5);
    }
    basic_speed = menu_speed_set * 10;
    task_and_speed_post(2, (uint8_t)basic_speed);

    until_distance(50);

    wait_for_condition(STOP_LINE);

    task_stop();

    time_count_flag = 0;
    basic_speed = 0;
    buzzer_bi_flag = 1;
    led_blink_flag = 71;
}
void Mission_6(void)
{
    led_blink_flag = 0;
    huifdu_flags.trace_mode = RIGHT_FIRST;
    basic_speed = 50;
    time_count_flag = 1;

    task_and_speed_post(6, (uint8_t)basic_speed);
    until_distance(130);

    task_and_speed_post_A0(6, 49);
    buzzer_bi_flag = 1;
    until_distance(200);

    wait_for_condition(STOP_LINE);
    //------------------------------------------ 3 1 2
    // until_distance(10);

    huifdu_flags.trace_mode = RIGHT_FIRST;
    until_distance(200);

    task_and_speed_post_A0(5, 49);
    buzzer_bi_flag = 1;
    until_distance(100);

    wait_for_condition(STOP_LINE);
    //---------------------------------------------- 1 3  2
    huifdu_flags.trace_mode = LEFT_FIRST;
    until_distance(170);

    task_and_speed_post_A0(5, 49);
    buzzer_bi_flag = 1;
    until_distance(100);

    wait_for_condition(STOP_LINE);

    // wait_for_condition(RIGHT_HAVE_LINE);

    // until_distance(150);

    // wait_for_condition(STOP_LINE);

    task_stop();
    task_stop_A0();

    time_count_flag = 0;
    basic_speed = 0;
    buzzer_bi_flag = 1;
    led_blink_flag = 71;
}
void Mission_5(void)
{
    led_blink_flag = 0;
    huifdu_flags.trace_mode = RIGHT_FIRST;
    basic_speed = 50;
    time_count_flag = 1;

    task_and_speed_post(5, (uint8_t)basic_speed);
    until_distance(130);

    task_and_speed_post_A0(5, 49);
    buzzer_bi_flag = 1;
    until_distance(200);

    wait_for_condition(STOP_LINE);
    // until_distance(10);

    huifdu_flags.trace_mode = LEFT_FIRST;
    until_distance(170);

    task_and_speed_post_A0(5, 49);
    buzzer_bi_flag = 1;
    until_distance(100);

    wait_for_condition(STOP_LINE);
    //----------------------------------------------
    // until_distance(200);

    // wait_for_condition(RIGHT_HAVE_LINE);

    // until_distance(150);

    // wait_for_condition(STOP_LINE);

    task_stop();
    task_stop_A0();

    time_count_flag = 0;
    basic_speed = 0;
    buzzer_bi_flag = 1;
    led_blink_flag = 71;
}

// void Mission_6(void)
// {
//     led_blink_flag = 0;
//     huifdu_flags.trace_mode = RIGHT_FIRST;
//     basic_speed = 50;
//     time_count_flag = 1;

//     task_and_speed_post(6, (uint8_t)basic_speed);
//     until_distance(230);

//     task_and_speed_post_A0(6, 49);
//     buzzer_bi_flag = 1;
//     until_distance(100);

//     wait_for_condition(STOP_LINE);
//     // until_distance(10);

//     huifdu_flags.trace_mode = LEFT_FIRST;
//     until_distance(170);

//     task_and_speed_post_A0(6, 49);
//     buzzer_bi_flag = 1;
//     until_distance(100);

//     wait_for_condition(STOP_LINE);
//     //----------------------------------------------
//     // until_distance(200);

//     // wait_for_condition(RIGHT_HAVE_LINE);

//     // until_distance(150);

//     // wait_for_condition(STOP_LINE);

//     task_stop();
//     task_stop_A0();

//     time_count_flag = 0;
//     basic_speed = 0;
//     buzzer_bi_flag = 1;
//     led_blink_flag = 71;
// }
