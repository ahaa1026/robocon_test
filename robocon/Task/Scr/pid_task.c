#include "pid_task.h"
#include "pid.h"
#include "encodor.h"
#include"pid_vec.h"
#include "pid_pos.h"
#include "pid_cascade.h"
#include "usart.h"
#include "debug.h"
#include "pid_vec_follow.h"
#include "pid_pos_follow.h"
#include "set_target.h"
#include "pid_pos_gyro.h"
extern float Yaw_Angle;

extern Pid vec_pid;
extern Pid pos_pid;
extern int cnt_mood;
extern float adc1_number0;
extern float speed_w_target;
extern Encodor encodor_pos_followed;
extern Encodor encodor;

void pid_task()
{


    if(cnt_mood%6==0)
    {
        pid_vec();//模式1，速度pid
        usart_printf("mood%d:vec_pid,%d,%d\n",cnt_mood+1,(int)encodor.speed.w,(int)vec_pid.variables.target);

    }


    /*
    else if(cnt_mood%6==1)
    {

        pid_pos();//模式2，单环位置pid
        usart_printf("mood%d:pos_pid,%d,%d\n",cnt_mood+1,(int)encodor.position.angle,(int)pos_pid.variables.target);
        //usart_printf("%d\n",(int)encodor.position.rotations*100);

    }



    else if(cnt_mood%6==2)
    {



        pid_pos();//模式2，单环位置pid
        usart_printf("mood%d:pos_pid,%d,%d,%d\n",cnt_mood+1,(int)encodor.position.angle,(int)pos_pid.variables.target,(int)(encodor.position.rotations*100));
        //pid_cascade();//模式3，串级pid，实现了稳定启动和平稳停下
        //usart_printf("mood%d:cascade_pid,%d,%d,%d,%d\n",cnt_mood+1,(int)encodor.speed.w,(int)encodor.position.angle,(int)pos_pid.variables.target,(int)vec_pid.variables.target);
    }


  /*
    else if(cnt_mood%6==3)
    {

        pid_vec_follow();//模式4，使用摇杆实现速度跟随
        usart_printf("mood%d:vec_follow,%d,%d\n",cnt_mood+1,(int)speed_w_target,(int)encodor.speed.w);

    }

*/
    else if(cnt_mood%6==4)
    {
        pid_pos_follow();//pid的主从电机位置跟随
        usart_printf("mood%d:pos_follow,%d,%d\n",cnt_mood+1,(int)encodor_pos_followed.position.angle,(int)encodor.position.angle);
    }

    else if(cnt_mood%6==5)
    {

        pid_pos_gyro();//使用陀螺仪实现位置跟随
        usart_printf("mood%d:pos_gyro,%d,%d\n",cnt_mood+1,(int)Yaw_Angle,(int)encodor.position.angle);
    }

}
