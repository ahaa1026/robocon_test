#include "encodor.h"
#include "gyro.h"
#include "main.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "dma.h"
uint8_t GYRO_Rx_ALL[GYRO_DATASIZE] = { 0 };//如果后期出现问题，这里改为GYRO_DATASIZE-1
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
float angle[3] = { 0 };
float vector[3] = { 0 };
static float angle_last;
static int32_t rotate_times;

//extern Encodor encodor;//记得不能跟电机本身的编码器存储数据的地方混用
//重新定义一个存储陀螺仪读取的数据的地方
float Yaw_Angle, Yaw_Vector;

void GYRO_UartInit(void)
{
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);//开启串口的空闲中断
    HAL_UART_Receive_DMA(&huart3, GYRO_Rx_ALL, GYRO_DATASIZE);
}


void GYRO_Data_Handler(void);
void GYRO_Receive_Data(void)
{
    if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);//清除空闲标志位，防止再次进入中断造成干扰
        HAL_UART_DMAStop(&huart3);//DMA方式停止接受数据

        GYRO_Data_Handler();

        uint8_t rx_len = GYRO_DATASIZE - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
        hdma_usart3_rx.Instance->CNDTR=33u;
        //__HAL_DMA_SET_COUNTER(&hdma_uart3_rx, GYRO_DATASIZE);

        //usart_printf("len:%d\r\n", rx_len);
        memset(GYRO_Rx_ALL, 0, 33);
        HAL_UART_Receive_DMA(&huart3, GYRO_Rx_ALL, GYRO_DATASIZE);
    }
}

//角度连续化函数
float PYRO_Angle_Consecutive(float angle_now)
{
    float angle_this;
    angle_this = angle_now;//结合angle_last=angle_this;
    if ((angle_this - angle_last) > 300)//角度连续化的同时进行的角度积累
        rotate_times--;
    if ((angle_this - angle_last) < -300)
        rotate_times++;
    angle_now = angle_this + rotate_times * 360.0f;
    //usart_printf("%d\r\n", rotate_times);
    angle_last=angle_this;//结合angle_this = angle_now;进行编码器读取的原始值的保存

    return angle_now;
}


void GYRO_Data_Handler(void)
{
    for (int8_t i = 0; i < 3; i++)
    {
        if (GYRO_Rx_ALL[11 * i] != 0x55)//wt61协议规定的数据帧头部为0x55
        {
            return;
        }
        switch (GYRO_Rx_ALL[i * 11 + 1])
        {

            /*
             速度数据包
        case 0x52://数据帧第二字节为0x52的时候读回的是数据类型为速度的数据帧
            vector[0] = (short)(((short)GYRO_Rx_ALL[i * 11 + 3] << 8) | GYRO_Rx_ALL[i * 11 + 2]) * 350.0f / 32768.0f;
            vector[1] = (short)(((short)GYRO_Rx_ALL[i * 11 + 5] << 8) | GYRO_Rx_ALL[i * 11 + 4]) * 350.0f / 32768.0f;
            vector[2] = (short)(((short)GYRO_Rx_ALL[i * 11 + 7] << 8) | GYRO_Rx_ALL[i * 11 + 6]) * 350.0f / 32768.0f;
            //Pih_Speed = speed[0] / 6.0f;
             Yaw_Vector= vector[2] / 6.0f;
            //usart_printf("%f,%f\r\n",Pih_Speed,Yaw_Speed);
            break;
            */
        case 0x53: //数据帧第二字节为0x53的时候读回的是数据类型为角度的数据帧


            //高八位按位或上低八位数据得到两字节的数据
            angle[0] = (short)(((short)GYRO_Rx_ALL[i * 11 + 3] << 8) | GYRO_Rx_ALL[i * 11 + 2]) * 180.0f / 32768.0f;//pitch俯仰角
            angle[1] = (short)(((short)GYRO_Rx_ALL[i * 11 + 5] << 8) | GYRO_Rx_ALL[i * 11 + 4]) * 180.0f / 32768.0f;//roll横滚角
            angle[2] = (short)(((short)GYRO_Rx_ALL[i * 11 + 7] << 8) | GYRO_Rx_ALL[i * 11 + 6]) * 180.0f / 32768.0f;//yaw航偏角

            Yaw_Angle = PYRO_Angle_Consecutive(angle[2]);//与编码器方向是一致的
            break;

        }
    }
}

