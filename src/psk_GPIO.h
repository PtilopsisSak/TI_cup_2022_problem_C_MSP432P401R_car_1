/*
 * @Author: ksn
 * @Date: 2022-07-20 14:32:48
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-21 20:56:25
 * @Version: Do not edit
 */
#ifndef PSK_GPIO_H_
#define PSK_GPIO_H_
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

// pin:x.y
#define GPIO_INIT_OU(x, y)      \
    {                           \
        P##x##DIR |= BIT##y##;  \
        P##x##OUT |= BIT##y##;  \
        P##x##OUT &= ~BIT##y##; \
    }

// pin:x.y
// ≈‰÷√…œ¿≠
#define GPIO_INIT_IN(x, y)      \
    {                           \
        P##x##DIR &= ~BIT##y##; \
        P##x##REN |= BIT##y##;  \
        P##x##OUT |= BIT##y##;  \
    }

#define GPIO_INIT_IN_NO_UP(x, y) \
    {                            \
        P##x##DIR &= ~BIT##y##;  \
        P##x##REN |= BIT##y##;   \
    }

// pin:x.y | z:h or l
#define GPIO_SET(x, y, z)           \
    {                               \
        if (z)                      \
            P##x##OUT |= BIT##y##;  \
        else                        \
            P##x##OUT &= ~BIT##y##; \
    }
#define GPIO_TOGGLE(x, y)      \
    {                          \
        P##x##OUT ^= BIT##y##; \
    }
#define GPIO_READ(x, y) \
    (P##x##IN & (0x01 << y))

#endif
