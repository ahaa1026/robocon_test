
//#include <cstring>
//#include <cstdlib>
#include "set_target.h"
#include "usart.h"
#include "pid.h"
#include "roboinit.h"
#include "string.h"
#include "stdlib.h"

float index_vec=0;
extern Pid vec_pid;
extern Pid pos_pid;
extern int cnt_mood;

char debugRvAll[DEBUG_RV_MXSIZE]={0};//存放串口1直接接收数据
char debugRvData[DEBUG_RV_MXSIZE]={0};//存放要传入的数据
char debugRvData_vec[DEBUG_RV_MXSIZE]={0};//存放要传入的数据

char *pEnd;
int start_flag = 0;

void Set_Target_UartInit()
{
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);//使能串口一的空闲中断
    HAL_UART_Receive_DMA(&huart1, (uint8_t*)debugRvAll, DEBUG_RV_MXSIZE);//串口的DMA接收数据
}

void Set_Target_UartIdleCallback(UART_HandleTypeDef *huart);
void Set_Target_UartIrqHandler(UART_HandleTypeDef *huart)
{
    if(huart->Instance == huart1.Instance)//判断是否是串口1
    {
        if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))//判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);//清楚空闲中断标志，防止会一直不断进入中断
            Set_Target_UartIdleCallback(huart);//调用中断处理函数
        }
    }
}

void Set_Target_UartIdleCallback(UART_HandleTypeDef *huart)
{
    float index=0;

    float index_vec=0;
    HAL_UART_DMAStop(huart);//停止本次DMA传输

    //将串口收到的数据进行处理，新的数组存放要传入的数据
    memcpy(debugRvData ,&debugRvAll[4], 5);//将后续模式3，pid串级以期望速度达到指定位置这个模式的指定位置存入debugRvData（模式1的速度和模式2的位置也传入这个地方）
    memcpy(debugRvData_vec ,&debugRvAll[9], 10);//将期望速度邪通过串口接收的存入debugRvData_vec

    //计算接收到的数据长度，接收到的数据长度等于数组的最大存储长度减去DMA空闲的数据区长度
    uint8_t data_length  = DEBUG_RV_MXSIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx);

    //电机启动
    if(debugRvAll[1] == MAOHAO && debugRvAll[2] == START)//如果串口接受的数据第二和第三字节分别为：和1，则把电机开始转动的标志位置1，在motor——task任务写了电机开始转动
    {
        start_flag = 1;
    }
    else if(debugRvAll[1] == MAOHAO && debugRvAll[2] == STOP)//如果串口接受的数据第二和第三字节分别为：和0，则把电机开始转动的标志位置0，在motor——task任务写了电机停止转动
    {
        start_flag = 0;
    }

    //速度环pid参数设置
    if(cnt_mood%5==0&&debugRvAll[0]==V_Target)//V_Target输入t即可，电机启动的输入形式为t：1 期望速度
                                                               //电机停止的输入形式为t：0
    {
        index=strtof(debugRvData,&pEnd);
        setPidTarget(&vec_pid,index);

    }


    else if(cnt_mood%5==1&&debugRvAll[0]==P_Target)
    {

        index=strtof(debugRvData,&pEnd);
        setPidTarget(&pos_pid,index);
    }


    else if(cnt_mood%5==2)
    {

        if(debugRvAll[0]==PV_P_Target)
        {
                //index=strtof(debugRvData,&debugRvAll[9]);//测试时发现这种把&debugRvAll[9]当作strtof的想法对函数的使用不正确
                index=strtof(debugRvData,&pEnd);//字符串转float类型，指定位置转为float类型（解决联合体的问题）
                index_vec=strtof(debugRvData_vec,&pEnd);//期望速度转为float类型（解决联合体的问题）

                //我在这里定义的计算公式：ramp_target_step为斜坡函数的步长，ramp_target_step/360为转数，ramp_target_step/360/（ramp_target_time+1）/（pid——task的任务周期）为转速，单位为r/ms
                //{ramp_target_step/360/（ramp_target_time+1）/（pid——task的任务周期）}*1000*60转换单位，为rpm。
                pos_pid.ramp_target_step=index_vec*360*2*5/(1000*60);
                vec_pid.ramp_target_step=index_vec*360*2*5/(1000*60);

                setPidTargetwithRamp(&pos_pid,index);
        }

    }
    memset(debugRvAll,0,data_length); //清零接收缓冲区
    data_length = 0;
    HAL_UART_Receive_DMA(huart, (uint8_t*)debugRvAll, DEBUG_RV_MXSIZE);
}

