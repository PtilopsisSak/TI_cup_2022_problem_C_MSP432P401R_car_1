/*
 * @Author: Ptisak
 * @Date: 2022-07-20 14:32:48
 * @LastEditors: Ptisak
 * @LastEditTime: 2022-07-31 10:24:04
 * @Version: Do not edit
 */

#include "psk_USART.h"
#include "psk_DELAY.h"

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
unsigned char printf_temp[200];
// 115200
const eUSCI_UART_ConfigV1 uartConfig =
    {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,                // SMCLK Clock Source
        26,                                            // BRDIV = 13
        0,                                             // UCxBRF = 0
        111,                                           // UCxBRS = 37
        EUSCI_A_UART_NO_PARITY,                        // No Parity
        EUSCI_A_UART_LSB_FIRST,                        // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,                     // One stop bit
        EUSCI_A_UART_MODE,                             // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
        EUSCI_A_UART_8_BIT_LEN                         // 8 bit data length
};
// 9600
const eUSCI_UART_ConfigV1 uartConfig_9600 =
    {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,                // SMCLK Clock Source
        312,                                           // BRDIV = 13
        8,                                             // UCxBRF = 0
        0,                                             // UCxBRS = 37
        EUSCI_A_UART_NO_PARITY,                        // No Parity
        EUSCI_A_UART_LSB_FIRST,                        // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,                     // One stop bit
        EUSCI_A_UART_MODE,                             // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
        EUSCI_A_UART_8_BIT_LEN                         // 8 bit data length
};
/* EUSCI A0 UART ISR - Echoes data back to PC host */
// void EUSCIA0_IRQHandler(void)
// {
//     uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

//     if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
//     {
//         // MAP_UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A0_BASE));
//         MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
//     }
// }
void EUSCIA1_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A1_BASE);

    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        // MAP_UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A1_BASE));
        MAP_UART_clearInterruptFlag(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
        // buzzer_bi_flag = 1;
    }
}
void EUSCIA2_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        // MAP_UART_transmitData(EUSCI_A2_BASE, MAP_UART_receiveData(EUSCI_A2_BASE));
        MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
        uart_handler();
    }
}
void EUSCIA3_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A3_BASE);

    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        // MAP_UART_transmitData(EUSCI_A3_BASE, MAP_UART_receiveData(EUSCI_A3_BASE));
        MAP_UART_clearInterruptFlag(EUSCI_A3_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
        jy901s_handler(MAP_UART_receiveData(EUSCI_A3_BASE));
    }
}

void USART0_DEBUG_INIT(void)
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                                   GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    MAP_UART_enableModule(EUSCI_A0_BASE);
    // MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    // MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
    // MAP_Interrupt_enableMaster();
}

void USART1_INIT(void)
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,
                                                   GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_UART_initModule(EUSCI_A1_BASE, &uartConfig);
    MAP_UART_enableModule(EUSCI_A1_BASE);
    MAP_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA1);
    MAP_Interrupt_enableMaster();
}

void USART2_INIT(void)
{
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
                                                   GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    //![Simple UART Example]
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A2_BASE);

    /* Enabling interrupts */
    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);
    MAP_Interrupt_enableMaster();
}
void USART3_INIT(void)
{
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9,
                                                   GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    //![Simple UART Example]
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A3_BASE, &uartConfig_9600);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A3_BASE);

    /* Enabling interrupts */
    MAP_UART_enableInterrupt(EUSCI_A3_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA3);
    MAP_Interrupt_enableMaster();
}

void USART0_DEBUG_SEND(unsigned char *ptr)
{
    while (*ptr != '\0')
    {
        MAP_UART_transmitData(EUSCI_A0_BASE, *ptr);
        ptr++;
    }
}
void USART2_SEND(unsigned char *ptr)
{
    while (*ptr != '\0')
    {
        MAP_UART_transmitData(EUSCI_A2_BASE, *ptr);
        ptr++;
    }
}
uint8_t inf[20] = {0xAA, 0x05, 0xAF, 0xF1, 0x08};
//上位机接收帧头         数据长度
void send_four_data(uint8_t frame, int16_t num1, int16_t num2, int16_t num3, int16_t num4) //向虚拟上位机发送数据
{

    uint8_t t;
    uint8_t sum = 0;
    inf[3] = frame;

    inf[5] = num1 >> 8;
    inf[7] = num2 >> 8;
    inf[9] = num3 >> 8;
    inf[11] = num4 >> 8;
    inf[6] = num1 & 0x00ff;
    inf[8] = num2 & 0x00ff;
    inf[10] = num3 & 0x00ff;
    inf[12] = num4 & 0x00ff;
    for (t = 0; t < 13; t++)
    {
        sum += inf[t];
        // uart_write_byte(UART_6, inf[t]);
        // MAP_UART_transmitData(EUSCI_A0_BASE, inf[t]);
        // delay_us(1);
    }
    // uart_write_byte(UART_6, sum); //发送校验和
    // MAP_UART_transmitData(EUSCI_A0_BASE, sum);
}
