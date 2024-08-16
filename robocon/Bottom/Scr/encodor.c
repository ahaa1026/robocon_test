#include "encodor.h"

#include "debug.h"
#include"tim.h"

void EncodorInit(Encodor *encodor,const Paramater paramater)
{
    //将传入的编码器参数赋给paramater
    encodor->paramater=paramater;

    encodor->counter.counter=0;
    encodor->counter.conuter_total=0;
    encodor->counter.counter_increase=0;

    encodor->position.rotations=0;
    encodor->position.distance=0;
    encodor->position.angle=0;

    encodor->speed.w=0;
    encodor->speed.v=0;

    encodor->direction=INIT;

}

void EncodorGetCounter(Encodor *encodor)
{
    //车轮的转动方向
    encodor->direction=__HAL_TIM_IS_TIM_COUNTING_DOWN(encodor->paramater.tim_hander);

    //得到编码器当前的计数值
    encodor->counter.counter=(int16_t)__HAL_TIM_GET_COUNTER(encodor->paramater.tim_hander);

    //计算编码器的差值
    encodor->counter.counter_increase=(int64_t)((int16_t)encodor->counter.counter-0);

    //计算编码器总共的计数值
    encodor->counter.conuter_total+=encodor->counter.counter;

    __HAL_TIM_SetCounter(encodor->paramater.tim_hander,0);//清空计数器的值，解决溢出问题
}


void EncodorGetPosition(Encodor *encodor)
{
    //使用公式,用encodor->counter.conuter_total计算车轮转过的圈数
    //公式：车轮走过的圈数=编码器的总计数/(倍频数*减速比*电机的分辨率)
    encodor->position.rotations=(double)encodor->counter.conuter_total/((double)encodor->paramater.multi_frequency*(double)encodor->paramater.reduction_speed*(double)encodor->paramater.resolution_ratio);
    //encodor->position.rotations=(double)0/((double)encodor->paramater.multi_frequency*(double)encodor->paramater.reduction_speed*(double)encodor->paramater.resolution_ratio);

    //计算车轮转过的角度
    encodor->position.angle=encodor->position.rotations*360;

    //计算车轮走过的距离
    encodor->position.distance=encodor->position.rotations*2.0*3.1415926535*encodor->paramater.r;

   // usart_printf("%d\n",(int)(encodor->position.rotations*100));

}


//计算车轮的转速函数，time为定时器周期
void EncodorGetSpeed(Encodor*encodor,int time)
{

    //计算角速度，单位为rpm
    encodor->speed.w=(double)encodor->counter.counter_increase/(((double)encodor->paramater.multi_frequency*(double)encodor->paramater.reduction_speed*(double)encodor->paramater.resolution_ratio))*1000*60/time;

    //计算线速度，单位为m/s
    encodor->speed.v=(double)(encodor->speed.w/60*2.0*3.1415926535*encodor->paramater.r);

}

