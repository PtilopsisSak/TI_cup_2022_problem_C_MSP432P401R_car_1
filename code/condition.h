/*
 * @Author: Ptisak
 * @Date: 2022-07-29 08:51:47
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-29 08:59:31
 * @Version: Do not edit
 */
#ifndef __COND_H
#define __COND_H
#include "psk_ALL.h"

#define NO_condition 0 //
                       //
// #define LEFT45 1          //车左??45°有白线时
// #define LEFT90 2          //
// #define LEFT135 3         //
//                           //
// #define RIGHT45 4         //
// #define RIGHT90 5         //车右??90°有白线时
// #define RIGHT135 6        //
//                           //
// #define UPHILL 7          //
// #define DOWNHILL 8        //车???在下坡的状态时    angle_y > 21.5f
// #define FLAT 9            //车???在平坦的状态时    angle_y > -5.0f && angle_y < 5.0f
//                           //
// #define LINEINMIDDLE 10   //激光中间???测到白线??  Laser_detect[3] || Laser_detect[4]
//                           //
// #define NOT_UPHILL 11     //
// #define NOT_DOWNHILL 12   //车不处在下坡的状态时  angle_y < 21.5f
// #define NOT_FLAT 13       //车不处在平坦的状态时  angle_y < -5.0f || angle_y > 5.0f
//                           //
// #define SEESAW 14         //上跷跷板??           angle_x - angle_y > 5.0f
// #define ANGLE_Y_CHANGE 15 //角度Y变化??, 允???变化范围??2°

// #define OBSTACLE 16 //遇到障???时??顶起来时

#define LEFT_HAVE_LINE 17
#define RIGHT_HAVE_LINE 18

// #define COLLISION 19 //碰撞开关???测到碰撞

#define STOP_LINE 20

extern int now_condition;
extern u8 condition_str[][7];

void wait_for_condition(int c); //等待条件出现退出函??

int is_condition(int c); //返回??否???于指定条件

#endif
