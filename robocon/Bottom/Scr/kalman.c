
#include "kalman.h"

void KalmanInit(Kalman *kalman,float Q,float R)
{
    //给定状态初值和误差协方差初值
    kalman->x_last=0;
    kalman->p_last=0;
    kalman->p_pior=0;

    //系数参数初始化
    kalman->A=1;
    kalman->B=0;
    kalman->H=1;

    kalman->Q=Q;
    kalman->R=R;

}

float KalmanFilter(Kalman* kalman,float date)//输入量为待滤波的数据
{

    //预测部分的实现
    kalman->x_pior=kalman->A*kalman->x_last;//先验:x_(k) = A*x(k-1)+B*u(k-1)
    kalman->p_pior=kalman->A*kalman->p_last+kalman->Q;//先厌误差协方差：p_(k) = A*p(k-1)*A'+Q

    //校正部分的实现：
    kalman->Kk=kalman->p_pior/(kalman->p_pior+kalman->R);//卡尔曼增益:Kk= p_(k)*H'/(H*p_(k)*H'+R)
    kalman->x_posterior_now=kalman->x_pior+kalman->Kk*(date-kalman->x_pior);//后验估计：x(k) = x_(k)+Kk*(Z(k)-H*x_(k))
    kalman->p_now=(1-kalman->Kk)*kalman->p_pior;//更新后验估计协方差：p(k) = (I-Kk*H)*p_(k)

    //更新
    kalman->p_last=kalman->p_now;
    kalman->x_last=kalman->x_posterior_now;

    //返回结果为此刻的后验估计值
    return kalman->x_posterior_now;
}


