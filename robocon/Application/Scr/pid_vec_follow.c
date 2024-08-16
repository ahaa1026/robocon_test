#include "pid_vec_follow.h"
#include "main.h"
#include "adc.h"
#include "pid.h"
#include "encodor.h"
#include "usart.h"
#include "debug.h"


float adc1_number0=0.0;
float adc2_number1=0.0;
float speed_w_target=0.0;


extern Pid vec_pid;
extern Encodor encodor;
extern int start_flag;

void pid_vec_follow(void)
{

    start_flag=1;//start_flag电机启动标志位为1时，电机才会开始转动

    //PidInit(&vec_pid,10,5,0,800,1000,3000,Normal_state);

    //ADC读取摇杆的数据
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,1);
    adc1_number0=HAL_ADC_GetValue(&hadc1);

   /* HAL_ADC_Start(&hadc2);
    HAL_ADC_PollForConversion(&hadc2,HAL_MAX_DELAY);
    adc2_number1=HAL_ADC_GetValue(&hadc2);
   */


    speed_w_target=2*200.0*adc1_number0/4000.0-200;//读取到的ADC值转换为目标速度
   /* if(speed_w_target>0)
    {
        speed_w_target-=60;//调试速度超幅度，这个解决方法无效
    }
*/


    if(speed_w_target<40&&speed_w_target>-40)//消除抖动
    {
        speed_w_target=0;
    }

    //速度pid
    setPidTarget(&vec_pid,speed_w_target);
    Pid_Update_Gamp(&vec_pid,encodor.speed.w);

    vec_pid.variables.output.output_total=PidGet(&vec_pid,encodor.speed.w,0.1f,200.0);



}