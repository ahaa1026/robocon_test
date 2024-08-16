#include "motor.h"
#include "tim.h"
#include "main.h"


//限制limit函数
float limit_float(float input1,float max_input1)
{
    if((input1>=-max_input1)&&(input1<=max_input1))
    {
        return input1;
    }
    else if(input1>max_input1)
    {
        return max_input1;
    }
    else
    {
        return -max_input1;
    }
}

int limit_int(int input,int max_input)
{
    if((input>=-max_input)&&(input<=max_input))
    {
        return input;

    }
    else if(input>max_input)
    {
        return max_input;
    }
    else
    {
        return -max_input;
    }
}


//设置电机转动的方向和pwm_speed的大小
//用pwm_speed的正负代表了电机旋转的方向
void Motor_Dirctiion_Speed(int pwm_speed)
{

    //电机速度>0或者<0，则AIN1_GPIO_Port和AIN2_GPIO_Port一个高电平一个低电平，需要后期根据轮子安装的方向在应用层调试修改
    if(pwm_speed>0)
    {
        HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,RESET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,SET);
        pwm_speed=limit_int(pwm_speed,1200);//限制最大的pwm输出，根据定时器分频计算得到
        __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,pwm_speed);//通过调整占空比改变电机转速

    }
    else if(pwm_speed<0)
    {
        HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,SET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,RESET);
        pwm_speed=limit_int(pwm_speed,1200);
        __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,-pwm_speed);
    }


    else//速度为0，停下
    {
        HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,SET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,SET);
    }
}
