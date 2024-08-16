#include "encodor.h"
#include "motor.h"
#include "pid.h"
#include "roboinit.h"
#include "kalman.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include"task.h"
#include"debug.h"
extern Encodor encodor;
Encodor encodor_pos_followed;
int cnt_mood=4;
extern Kalman kalman_pos_follow;


Pid vec_pid;
Pid pos_pid;
Encodor encodor;
/*
float expect_speed=200;
float expect_speed_division;
float expect_speed_rise;
float expect_speed_dec;
int cnt=1;
*/

void roboinit()
{

    Paramater encoder_paramater={&htim3,4,30,500,0.0425};
    EncodorInit(&encodor,encoder_paramater);//编码器初始化

    Paramater encoder_paramater_pos_follow={&htim4,4,30,500,0.0425};
    EncodorInit(&encodor_pos_followed,encoder_paramater_pos_follow);//位置跟随的主动电机的编码器初始化

   PidInit(&vec_pid,30,0.6,0,800,1000,3000,Normal_state);//单独速度环的参数//30,0.6
    //调参遇到的问题：以下两点之间的冲突：1.积分饱和。2.kp过大控制不了较低转速，电机会抖动



    /*
    PidInit(&vec_pid,43,0.12,0,800,1000,3000,Normal_state);
   //PidInit(&pos_pid,3.8,0.0,0.45,1000,400,3000,Normal_state);

    PidInit(&pos_pid,1.5,0.0,0.5,1000,400,3000,Normal_state);
*/


    //KalmanInit(&kalman_pos_follow,1,9);//卡尔曼滤波器调参数



    //usart_printf("%d,%.2f,%.2f\n",cnt_mood,encodor.speed.w,vec_pid.variables.output.output_total);

    //vTaskDelete(NULL);

    //osThreadExit();
}


