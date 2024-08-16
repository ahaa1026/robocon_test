#include "pid_pos_follow.h"
#include "encodor.h"
#include "pid.h"
#include "kalman.h"
extern  Encodor encodor_pos_followed;



extern Pid vec_pid;
extern Pid pos_pid;
extern  Encodor encodor;
extern int start_flag;

Kalman kalman_pos_follow;


void pid_pos_follow(void)
{

    start_flag=1;

    //编码器读取主电机的位置
    EncodorGetCounter(&encodor_pos_followed);
    EncodorGetPosition(&encodor_pos_followed);
    EncodorGetSpeed(&encodor_pos_followed,5);//记得这里的时间为5没有错误，虽然encoder任务线程的软件定时器周期时间为1，但是这里是在pid——task里写的位置跟随

   // encodor_pos_followed.position.angle=KalmanFilter(&kalman_pos_follow,encodor_pos_followed.position.angle);

    //外环
    setPidTarget(&pos_pid,encodor_pos_followed.position.angle);
    Pid_Update_Gamp(&pos_pid,encodor.position.angle);
    pos_pid.variables.output.output_total=PidGet(&pos_pid,encodor.position.angle,0.1f,200.0);

    //内环
    setPidTarget(&vec_pid,pos_pid.variables.output.output_total);
    Pid_Update_Gamp(&vec_pid,encodor.speed.w);//实现斜坡函数，单独封装
    vec_pid.variables.output.output_total=PidGet(&vec_pid,encodor.speed.w,0.1f,200);//实现内环速度pid



}