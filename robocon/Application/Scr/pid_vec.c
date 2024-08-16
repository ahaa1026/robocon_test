#include "encodor.h"
#include "pid.h"
#include "motor.h"


extern Encodor encodor;
extern Pid vec_pid;

extern int start_flag;
void pid_vec()
{
    //start_flag=1;



   // setPidTarget(&vec_pid,200);
    Pid_Update_Gamp(&vec_pid,encodor.speed.w);
    vec_pid.variables.output.output_total=PidGet(&vec_pid,encodor.speed.w,0.1f,200.0);

}
