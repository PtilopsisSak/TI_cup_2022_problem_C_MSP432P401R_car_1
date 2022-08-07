/*
 * @Author: ksn
 * @Date: 2022-07-27 15:34:35
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-31 12:30:43
 */
#include "menu.h"
#include "psk_GPIO.h"
#include "psk_OLED.h"

#define KEY1 GPIO_READ(1, 1)
#define KEY2 GPIO_READ(1, 4)

uint8_t pointer = 2;
uint8_t menu_speed_set = 3;

void mission_start(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "Mission running", 8);
    led_blink_flag = 0;
    basic_speed = 0;
    time_count_flag = 1;
    encode_total = 0;
}

void mission_fin(void)
{
    time_count_flag = 0;
    basic_speed = 0;
    buzzer_bi_flag = 1;

    OLED_ShowString(0, 0, "Mission Complete", 8);
    zf_sprintf(printf_temp, "%d.%d s", mission_time_count / 100, mission_time_count % 100);
    OLED_ShowString(0, 5, printf_temp, 8);

    zf_sprintf(printf_temp, "%d cm  ", (int)distance_calc());
    OLED_ShowString(0, 6, printf_temp, 8);

    KEY_Scan();
    mission_time_count = 0;
    led_blink_flag = 1000;
    encode_total = 0;
    OLED_Clear();
}
void KEY_INIT(void)
{
    GPIO_INIT_IN(1, 1);
    GPIO_INIT_IN(1, 4);
}

int KEY_Scan(void)
{
    while (1)
    {
        if (!KEY1)
        {
            while (1)
            {
                if (KEY1)
                {
                    return 1;
                }
            }
        }
        else if (!KEY2)
        {
            while (1)
            {
                if (KEY2)
                {
                    return 2;
                }
            }
        }
    }
}

void UI_MainMenu(void)
{
    OLED_Clear();
    OLED_ShowString(2, 0, "Mission 1", 8);
    OLED_ShowString(2, 1, "Mission 2", 8);
    OLED_ShowString(2, 2, "Mission 3", 8);
    OLED_ShowString(2, 3, "Mission 4", 8);
    OLED_ShowString(2, 4, "Mission 5", 8);
    OLED_ShowString(2, 5, "Mission 6", 8);

    OLED_ShowString(82, pointer, "<-", 8);
}
void CTRL_MainMenu(void)
{
    while (1)
    {
        UI_MainMenu();
        switch (KEY_Scan())
        {
        case 1:
            pointer = (pointer + 1) % 6;
            UI_MainMenu();
            break;
        case 2:
            switch (pointer)
            {
            case 0:
                mission_start();
                // Mission 1
                Mission_1();
                mission_fin();
                break;
            case 1:
                mission_start();
                // Mission 2
                Mission_2();
                mission_fin();
                break;
            case 2:
                UI_Mission3();
                CTRL_Mission(Mission_3);
                break;
            case 3:
                UI_Mission3();
                CTRL_Mission(Mission_4);

                // mission_start();
                // // Mission 4
                // Mission_4();
                // mission_fin();
                break;
            case 4:
                mission_start();
                // Mission 5
                Mission_5();
                mission_fin();
                break;
            case 5:
                mission_start();
                // Mission 5
                Mission_6();
                mission_fin();
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void UI_Mission3(void)
{
    OLED_Clear();
    OLED_ShowString(10, 3, "Speed: ", 8);
    OLED_ShowNum(64, 3, menu_speed_set, 2, 8);
    OLED_ShowString(82, 3, "dm/s", 8);
}

void CTRL_Mission(void f(void))
{
    while (1)
    {
        switch (KEY_Scan())
        {
        case 1:
            menu_speed_set = (menu_speed_set + 1 - 3) % 11 + 3;
            UI_Mission3();
            break;
        case 2:
            mission_start();
            // Mission3
            // Mission_3();
            f();
            mission_fin();
            return;
            break;
        default:
            break;
        }
    }
}
