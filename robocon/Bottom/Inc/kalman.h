#ifndef __KALMAN_H_
#define __KALMAN_H_


typedef struct {
    //状态量
    float x_pior;//先验估计
    float x_posterior_now;//后验估计,也是此时刻滤波的输出值
    float x_last;//上一个时刻的最优结果

    //误差的协方差
    float p_pior;//先验误差协方差
    float p_now;//当前时刻最优结果的协方差
    float p_last;//上一时刻最优结果的协方差

    //卡尔曼增益
    float Kk;//kalman gain

    //系统参数
    float A;
    float B;
    float Q;//系统噪声协方差
    float R;//测量噪声协方差
    float H;
}Kalman;

void KalmanInit(Kalman *kalman,float Q,float R);
float KalmanFilter(Kalman* kalman,float date);


void kalman_init(void);




#endif
