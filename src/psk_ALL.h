/*
 * @Author: Ptisak
 * @Date: 2022-06-15 11:36:20
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-29 09:49:54
 * @Version: Do not edit
 */

#ifndef PSK_ALL_H_
#define PSK_ALL_H_

typedef unsigned char uint8;       // 无符号  8 bits
typedef unsigned short int uint16; // 无符号 16 bits
typedef unsigned long int uint32;  // 无符号 32 bits
typedef unsigned long long uint64; // 无符号 64 bits

typedef char int8;       // 有符号  8 bits
typedef short int int16; // 有符号 16 bits
typedef long int int32;  // 有符号 32 bits
typedef long long int64; // 有符号 64 bits

typedef volatile uint8 vuint8;   // 易变性修饰 无符号  8 bits
typedef volatile uint16 vuint16; // 易变性修饰 无符号 16 bits
typedef volatile uint32 vuint32; // 易变性修饰 无符号 32 bits
typedef volatile uint64 vuint64; // 易变性修饰 无符号 64 bits

typedef volatile int8 vint8;   // 易变性修饰 有符号  8 bits
typedef volatile int16 vint16; // 易变性修饰 有符号 16 bits
typedef volatile int32 vint32; // 易变性修饰 有符号 32 bits
typedef volatile int64 vint64; // 易变性修饰 有符号 64 bits

#include "math.h"

#include "psk_GPIO.h"
#include "psk_DELAY.h"
#include "psk_USART.h"
#include "psk_INTERRUPT.h"
#include "psk_OLED.h"
//#include "psk_fout.h"
#include "psk_OLED_type.h"
#include "psk_PWM.h"
#include "psk_motor_control.h"
#include "pid.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_PRINTF.h"
#include "psk_aht10.h"
#include "posget.h"
#include "servo.h"
#include "huidu.h"
#include "led_beep.h"
#include "mission.h"

#include "menu.h"
#include "ycz_communicate.h"
#include "ycz_distance.h"
#include "jy901s.h"
#include "ultrasonic.h"
#include "condition.h"

#define map(x, in_min, in_max, out_min, out_max) \
    (((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min))

#define BEEP_ON GPIO_SET(6, 4, 1);
#define BEEP_OFF GPIO_SET(6, 4, 0);
#define BEEP_TOGGLE GPIO_TOGGLE(6, 4);
#define LED_ON GPIO_SET(5, 5, 0);
#define LED_OFF GPIO_SET(5, 5, 1);
#define LED_TOGGLE GPIO_TOGGLE(5, 5);
#define LED1_ON GPIO_SET(3, 5, 0);
#define LED1_OFF GPIO_SET(3, 5, 1);
#define LED1_TOGGLE GPIO_TOGGLE(3, 5);
#endif

//                                   ;LCSsLL.
//                                  ;@M. .;5M@@L
//                                   ,@2 L@M#EPs               ,;LLtsPdG$MMM@c
//        ;;;,. .             ;;;:L99SP@MMML:.;9MMCLGR$$$#M@M@M$$92#M@M@M@M@M2
//        jM@MMMMMMM$MM$$M@MMM$R2MM;       ,,    #@,     LL.          LM@MG M2
//         RM5M@M;        ..     M3        @MDBT; j@ML,                LM3  M3
//          @L E@L              .M  9O     j@; LSDEO@MMM@,            SML   M;
//          c@   @M2             @;EM@MS;   ;ML                      @M.   9M  L;
//           MM   .R@S           9@M  ;O@M@$sM@ML               6L .@M     MJ LMMM@;
//           .M;     MM           @@       .;;;L;           .;sOM@L ;@M;  S@.5M@M@;
//            2M    EML            ML            6     ;@M@M@M@M@M@T  cM@:ME   CR
//             MM  @M             C@             @#     M@M@MBsL,.L@$   s@M
//              MM@2              MM             @M:     ;L;.;L5$MMM@M;   @t
//             LMC          ;  $J M@             LM@L  L  L@MMM@MMM$5tMM.  @B
//         ;3M@ML;j9Gd@M;   LMMM@MdM3M@M;        L@$@MRMMM;;@$CL,      c@E. PMs
//         ,Tj;RM;;  ;M2 ,C@M@M@MMEMM@.LM@RL.     M6 JMCM@G.        L;;.;6M@RL$@9;:
//             MJ  LMMMdDM@L,LdMO:;LLM@  ,J$@M@OJL@M   ,TM@MMM655d2@@M@ct59M6CsMMM3
//            MM   LT;:  M$ j@;       MO      ;LcL.;Lc$MM@Oc;   M@.     c@L@
//           t@          c@ M:        LM         ;@MMM@j        M;      @@ M;
//          ;@,           B#M;        c@          TtO@M@M@M@M$tM@   ,   MB @M
//         J@@LDMM     D@5;M@MJ      T@s   ;;LLL.;L;    .M, .5@M   M$M; @#  M;
//        M@MtL ,@B    CM@G;2L$@OSSs#@; LM3s3TjDDDLdM    6  T@MLM2;M LM EM  $@
//   ;@;  ,     @MMML  :@;@s            M@          @C    COJO@  BDMd @M@M;  M$
//   @M@ML     RM  LMMJ M2 6M#;         ,@,        3E     ;Ls@MMd:.#djc6M@;   Ms
//  2MMM@     @M     ;O@MMT  cPD9CL: .;;,L5c3PS2S2Cs;;c9M@5L;M@M32@ML    M; L ;MP
//     :L  ;d@T          .,   L@MMMMMMM@M@MMM@M@MMMMM@M@M3  M@Md  L@M    ML MMs@M@$;
//      :M@M$; ;L              M@M@M@MMM@M@M@MMM@MMM@M@M@L   @MMM3L3MM5@@M; ;M   ,;,
//        ;;;;L@@           ;T2$M@MMM@MMMMMMM@M@M@M@MMMMM@MMO@MMM@M@M@M@M@;  @9
//            DM          TM@M@M@M@MMMMM@MMMMM@MMMMM@M@M@M@MMMMMMM@M@M@M@M   9M
//            M;         $MMM@M@M@M@M@M@MMM@MMMMM@M@M@M@MMMMM@M@M@M@M@MMM@   TM
//            ML;LLLLL;J@M@M@M@MMMMM@M@MMM@M@M@MMM@MMM@M@M@M@M@M@M@M@MMMMB   M$
