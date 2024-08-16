#include "encodor.h"

#include "debug.h"
#include"tim.h"

void EncodorInit(Encodor *encodor,const Paramater paramater)
{
    //������ı�������������paramater
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
    //���ֵ�ת������
    encodor->direction=__HAL_TIM_IS_TIM_COUNTING_DOWN(encodor->paramater.tim_hander);

    //�õ���������ǰ�ļ���ֵ
    encodor->counter.counter=(int16_t)__HAL_TIM_GET_COUNTER(encodor->paramater.tim_hander);

    //����������Ĳ�ֵ
    encodor->counter.counter_increase=(int64_t)((int16_t)encodor->counter.counter-0);

    //����������ܹ��ļ���ֵ
    encodor->counter.conuter_total+=encodor->counter.counter;

    __HAL_TIM_SetCounter(encodor->paramater.tim_hander,0);//��ռ�������ֵ������������
}


void EncodorGetPosition(Encodor *encodor)
{
    //ʹ�ù�ʽ,��encodor->counter.conuter_total���㳵��ת����Ȧ��
    //��ʽ�������߹���Ȧ��=���������ܼ���/(��Ƶ��*���ٱ�*����ķֱ���)
    encodor->position.rotations=(double)encodor->counter.conuter_total/((double)encodor->paramater.multi_frequency*(double)encodor->paramater.reduction_speed*(double)encodor->paramater.resolution_ratio);
    //encodor->position.rotations=(double)0/((double)encodor->paramater.multi_frequency*(double)encodor->paramater.reduction_speed*(double)encodor->paramater.resolution_ratio);

    //���㳵��ת���ĽǶ�
    encodor->position.angle=encodor->position.rotations*360;

    //���㳵���߹��ľ���
    encodor->position.distance=encodor->position.rotations*2.0*3.1415926535*encodor->paramater.r;

   // usart_printf("%d\n",(int)(encodor->position.rotations*100));

}


//���㳵�ֵ�ת�ٺ�����timeΪ��ʱ������
void EncodorGetSpeed(Encodor*encodor,int time)
{

    //������ٶȣ���λΪrpm
    encodor->speed.w=(double)encodor->counter.counter_increase/(((double)encodor->paramater.multi_frequency*(double)encodor->paramater.reduction_speed*(double)encodor->paramater.resolution_ratio))*1000*60/time;

    //�������ٶȣ���λΪm/s
    encodor->speed.v=(double)(encodor->speed.w/60*2.0*3.1415926535*encodor->paramater.r);

}

