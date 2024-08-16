#ifndef __ENCODOR_H
#define __ENCODOR_H

#include "stdint.h"
#include "tim.h"
//定义参数结构体
typedef struct
{
    TIM_HandleTypeDef* tim_hander;
    uint8_t multi_frequency;//编码器倍频数
    float reduction_speed;//电机减速比
    uint16_t resolution_ratio;//电机的分辨率
    float r;//车轮半径，用于计算行驶的距离

}Paramater;

//定义使用的计时器tim3的计数器结构体
typedef struct
{
    int16_t counter;//__HAL_TIM_GET_COUNTER得到的结果是uint_t,为了使结果连续，强制类型转换
    int64_t  conuter_total;//总计数值
    int64_t  counter_increase;//计数值增量

}Counter;

//定义编码器得到的电机的位置结构体
typedef struct
{

    double rotations;//由counter计算得到
    double distance;//车轮走的距离
    double angle;//车轮转过的角度

}Position;


//定义编码器得到的电机的速度结构体
typedef struct
{
    //由counter_increase计算得到
    double v;//线速度
    double w;//角速度
}Speed;


typedef enum
{
    FORWARD=0,
    BACKWARD=1,
    INIT=2
}Direction;


typedef struct
{
    Paramater paramater;
    Counter counter;
    Position position;
    Speed speed;
    Direction direction;


}Encodor;


void EncodorInit(Encodor *encodor,const Paramater paramater);

void EncodorGetCounter(Encodor *encodor);

void EncodorGetPosition(Encodor *encodor);

void EncodorGetSpeed(Encodor*encodor,int time);



#endif
