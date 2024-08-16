#include "debug.h"
#include"stdint.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>

#define TX_BUF_SIZE 512
uint8_t send_buf[TX_BUF_SIZE];






//重定向串口打印函数
void usart_printf(const char* format, ...)
{
    va_list args;
    uint32_t length;
    va_start(args, format);

    length = vsnprintf((char*)send_buf, TX_BUF_SIZE, (const char*)format, args);

    va_end(args);

    HAL_UART_Transmit_DMA(&huart1, (uint8_t*)send_buf, length);
}

