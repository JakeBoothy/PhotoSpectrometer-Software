#include <stdio.h>

#include "fsl_uart.h"
#include "uart.h"

void uart_init()
{
    uart_config_t config;
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200u;
    config.enableRx = true;
    config.enableTx = true;
    UART_Init(UART1, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));//Bus clock
    UART_EnableRx(UART1,true);
    UART_EnableTx(UART1,true);
}

void uart_write(uint8_t* data, size_t length)
{
    UART_WriteBlocking(UART1,data,length);
}

uint8_t uart_read_byte()
{
    uint8_t b = UART_ReadBlocking(UART1,&b,1);
    return b;
}
