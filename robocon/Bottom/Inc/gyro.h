#ifndef __GYRO_H
#define __GYRO_H

//#include "stm32f1xx_hal.h"
//#include "stm32f1xx_hal_dma.h"
#define GYRO_DATASIZE 33u

void GYRO_UartInit(void);
float PYRO_Angle_Consecutive(float angle_now);
void GYRO_Data_Handler(void);

void GYRO_Receive_Data(void);







#endif
