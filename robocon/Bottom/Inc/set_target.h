#ifndef __DEBUG_TASK_H
#define __DEBUG_TASK_H

//#include "stm32f1xx_hal_uart.h"
#include "usart.h"
#define DEBUG_RV_MXSIZE 255

void Set_Target_UartInit();


void Set_Target_UartIdleCallback(UART_HandleTypeDef *huart);
void Set_Target_UartIrqHandler(UART_HandleTypeDef *huart);
#define START 0x31//电机启动标志位
#define STOP  0x30//电机截止标志位
#define MAOHAO 0x3A//宏定义冒号

#define V_Target 0x74//输入为小写的t
#define P_Target 0x74
#define PV_P_Target 0x70//输入为小写的p
#define PV_V_Target 0x76//输入为小写的v





#endif
