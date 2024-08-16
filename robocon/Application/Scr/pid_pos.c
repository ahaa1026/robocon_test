#include "encodor.h"
#include "pid.h"
#include "motor.h"


extern Encodor encodor;
extern Pid pos_pid;



void pid_pos(void)
{

    //PidInit(&pos_pid,3.8,0.0,0.45,1000,400,3000,Normal_state);

    Pid_Update_Gamp(&pos_pid,encodor.position.angle);
    pos_pid.variables.output.output_total=PidGet(&pos_pid,encodor.position.angle,0.1f,200.0);

}
