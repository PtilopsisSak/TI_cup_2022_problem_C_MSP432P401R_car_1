/*
 * @Author: Ptisak
 * @Date: 2022-07-23 20:02:32
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-28 22:44:41
 * @Version: Do not edit
 */
#include "psk_ALL.h"
#ifndef PSK_huidu
#define PSK_huidu
extern float huidu_error;

enum mode
{
    NONE,
    NORMAL,
    LEFT_FIRST,
    RIGHT_FIRST
};
typedef struct
{
    enum mode trace_mode;
    uint8_t is_end;

} huifdu_flags_t;
extern huifdu_flags_t huifdu_flags;
int sensor_read(int8_t i);
void huidu_io_init(void);
void huidu_get_error(void);

void clear_end_flag(void);
#endif