#include "pid_pos_gyro.h"
#include "pid.h"
#include "encodor.h"



extern Pid pos_pid;
extern float Yaw_Angle;
extern  Encodor encodor;
extern Pid pos_pid;
extern int start_flag;
extern Pid vec_pid;





void pid_pos_gyro(void)
{

    start_flag=1;

    //外环
    setPidTarget(&pos_pid,Yaw_Angle);
    Pid_Update_Gamp(&pos_pid,encodor.position.angle);
    pos_pid.variables.output.output_total=PidGet(&pos_pid,encodor.position.angle,0.1f,200.0);

    //内环
    setPidTarget(&vec_pid,pos_pid.variables.output.output_total);
    Pid_Update_Gamp(&vec_pid,encodor.speed.w);//实现斜坡函数，单独封装
    vec_pid.variables.output.output_total=PidGet(&vec_pid,encodor.speed.w,0.1f,200);//实现内环速度pid




}
