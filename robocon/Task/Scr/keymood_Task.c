#include "debug.h"
#include "keymood_task.h"
#include"gpio.h"
#include"FreeRTOS.h"
#include "task.h"
#include "encodor.h"
#include "pid.h"
extern Pid vec_pid;
extern Pid pos_pid;
extern int cnt_mood;
extern int flag1;
extern int start_flag;
extern Encodor encodor;
void keymood_task()
{
    if(HAL_GPIO_ReadPin(MOOD_GPIO_Port,MOOD_Pin)==0)
    {
        //bug：
       //这是一个很难找的bug，模式一切换到模式二写了编码器值赋值为0的程序但是编码器数值没有清空
       //原因是电机还在微幅度旋转
        start_flag=0;

        while(HAL_GPIO_ReadPin(MOOD_GPIO_Port,MOOD_Pin)==0)
        {
            vTaskDelay(1);

        }
        cnt_mood++;
        start_flag=0;


        encodor.counter.counter=0;
        encodor.counter.counter_increase=0;
        encodor.counter.conuter_total=0;//清空计数器
        encodor.position.rotations=0;
        __HAL_TIM_SetCounter(encodor.paramater.tim_hander,0);//清空计数器的值，解决溢出问题
        encodor.position.rotations=(double)0/((double)encodor.paramater.multi_frequency*(double)encodor.paramater.reduction_speed*(double)encodor.paramater.resolution_ratio);


        EncodorGetCounter(&encodor);
        EncodorGetPosition(&encodor);
        EncodorGetSpeed(&encodor,1);

        pos_pid.variables.target=0;
        vec_pid.variables.target=0;


        //usart_printf("%d\n",(int)encodor.position.angle);
        if(cnt_mood==0)
        {
            PidInit(&vec_pid,30,0.6,0,800,1000,3000,Normal_state);
            //调参遇到的问题：以下两点之间的冲突：1.积分饱和。2.kp过大控制不了较低转速，电机会抖动
        }

        if (cnt_mood==1)
        {
            //PidInit(&vec_pid,20,0.08,0,800,1000,3000,Normal_state);//单独速度环的参数
            PidInit(&pos_pid,4.55,0.0,0.45,1000,400,3000,Normal_state);//单独的位置环参数


        }
        else if(cnt_mood==2)
        {
            //串级pid（以期望速度到达指定位置）实现了缓慢启动和平稳停下的pid参数
            PidInit(&vec_pid,43,0.19,0,800,1000,3000,Normal_state);//ki原来0.12
            PidInit(&pos_pid,1.5,0.0,0.5,1000,400,3000,Normal_state);

        }
        else  if(cnt_mood==3)
        {
            PidInit(&vec_pid,8,0.5,0,800,1000,3000,Normal_state);//摇杆的速度跟随的参数;
        }

        else if(cnt_mood==4)
        {
            PidInit(&vec_pid,20,0.08,0,800,1000,3000,Normal_state);
            PidInit(&pos_pid,5.435,0.001,3,1000,400,3000,Normal_state);//主从电机的位置跟随的pid参数
            //这个参数需要重新调整
        }



        else if(cnt_mood==5)
        {
            PidInit(&vec_pid,21,0.49,0,800,1000,3000,Normal_state);
            PidInit(&pos_pid,7.835,0.0025,0,1000,400,3000,Normal_state);//陀螺仪的的位置跟随的pid参数

            //PidInit(&pos_pid,3.8,0.0,0.45,1000,400,3000,Normal_state);
       }

        else if(cnt_mood==6)
        {
            cnt_mood=0;
        }

    }
}
