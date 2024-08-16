//实现通过串级pid的位置控制，实现了电机以期望速度转到了指定位置
//通过随着周期对速度环和位置环的设定目标不断增长斜坡作用实现了电机的缓慢启动和平稳停下

#include "encodor.h"
#include "pid.h"
#include "motor.h"


extern Encodor encodor;
extern Pid pos_pid;
extern Pid vec_pid;



void pid_cascade(void)
{
    //外环的位置pid
    Pid_Update_Gamp(&pos_pid,encodor.position.angle);
    pos_pid.variables.output.output_total=PidGet(&pos_pid,encodor.position.angle,0.1f,300.0);

    //setPidTarget(&vec_pid,pos_pid.variables.output.output_total);
    //内环的速度pid
    setPidTargetwithRamp(&vec_pid,pos_pid.variables.output.output_total);
    Pid_Update_Gamp(&vec_pid,encodor.speed.w);//实现斜坡函数，单独封装
    vec_pid.variables.output.output_total=PidGet(&vec_pid,encodor.speed.w,0.1f,200);//实现内环速度pid
}
