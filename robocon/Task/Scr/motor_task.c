#include "motor.h"
#include "pid.h"
extern int cnt_mood;
extern Pid vec_pid;
extern Pid pos_pid;
#include "debug.h"
#include"encodor.h"
#include "set_target.h"

extern int start_flag;
extern Encodor encodor;

void motor_task()
{
    if(start_flag==1)//电机开启的标志位为1，开始转动
    {
        if(cnt_mood%6==0)
    {
        //Motor_Dirctiion_Speed(500);
        Motor_Dirctiion_Speed(vec_pid.variables.output.output_total);
        //usart_printf("%d,%.2f,%.2f\n",cnt_mood,encodor.speed.w,vec_pid.variables.output.output_total);
    }


    else if(cnt_mood%6==1)
    {
        Motor_Dirctiion_Speed(pos_pid.variables.output.output_total);

    }

    else if(cnt_mood%6==2)
    {
        Motor_Dirctiion_Speed(vec_pid.variables.output.output_total);

    }

    else if(cnt_mood%6==3)
    {

        Motor_Dirctiion_Speed(vec_pid.variables.output.output_total);

    }
    else if(cnt_mood%6==4)
    {

        Motor_Dirctiion_Speed(pos_pid.variables.output.output_total);

    }



    else if(cnt_mood%6==5)
    {

        Motor_Dirctiion_Speed(pos_pid.variables.output.output_total);

    }
        
    }

    else
    {
        Motor_Dirctiion_Speed(0);
    }






}