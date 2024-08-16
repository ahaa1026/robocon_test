/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "encoder_task.h"
#include"keymood_task.h"
#include"motor_task.h"
#include"pid_task.h"

#include "usart.h"
#include"motor.h"
#include"encodor.h"
#include "debug.h"
#include "pid.h"
#include "roboinit.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern Encodor encodor;
extern Pid vec_pid;
extern Pid pos_pid;
/*
osThreadId_t PidTaskHandle;
osThreadId_t EncodorTaskHandle;
osThreadId_t MotorTaskHandle;;
osThreadId_t KeyMoodTaskHandle;


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for PidTask */
osThreadId_t PidTaskHandle;
const osThreadAttr_t PidTask_attributes = {
  .name = "PidTask",
  .stack_size = 528 * 4,
  .priority = (osPriority_t) osPriorityNormal3,
};
/* Definitions for EncoderTask */
osThreadId_t EncoderTaskHandle;
const osThreadAttr_t EncoderTask_attributes = {
  .name = "EncoderTask",
  .stack_size = 528 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for KeyMoodTask */
osThreadId_t KeyMoodTaskHandle;
const osThreadAttr_t KeyMoodTask_attributes = {
  .name = "KeyMoodTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 528 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Pid_Task(void *argument);
void Encoder_Task(void *argument);
void KeyMood_Task(void *argument);
void Motor_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of PidTask */
  PidTaskHandle = osThreadNew(Pid_Task, NULL, &PidTask_attributes);

  /* creation of EncoderTask */
  EncoderTaskHandle = osThreadNew(Encoder_Task, NULL, &EncoderTask_attributes);

  /* creation of KeyMoodTask */
  KeyMoodTaskHandle = osThreadNew(KeyMood_Task, NULL, &KeyMoodTask_attributes);

  /* creation of MotorTask */
  MotorTaskHandle = osThreadNew(Motor_Task, NULL, &MotorTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  portENABLE_INTERRUPTS();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Pid_Task */
/**
* @brief Function implementing the PidTask thread.
* @param argument: Not used
* @retval None
*/
extern int cnt_mood;

/* USER CODE END Header_Pid_Task */
void Pid_Task(void *argument)
{
  /* USER CODE BEGIN Pid_Task */
  /* Infinite loop */
  portTickType CurrentTime1;

  for (;;)
  {
    CurrentTime1=xTaskGetTickCount();
    pid_task();
    //usart_printf("1\n");
    //usart_printf("%d,%d\n",cnt_mood,(int)encodor.speed.w);
    vTaskDelayUntil(&CurrentTime1,5);
  }

  vTaskDelete(NULL);

  /* USER CODE END Pid_Task */
}

/* USER CODE BEGIN Header_Encoder_Task */
/**
* @brief Function implementing the EncoderTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Encoder_Task */
void Encoder_Task(void *argument)
{
  /* USER CODE BEGIN Encoder_Task */
  /* Infinite loop */
 portTickType CurrentTime2;
  for (;;)
  {
    CurrentTime2=xTaskGetTickCount();

    encorder_task();
    //usart_printf("2\n");
    //usart_printf("%d,%.2f,%.2f\n",cnt_mood,encodor.speed.w,vec_pid.variables.output.output_total);
    //Motor_Dirctiion_Speed(1000);
    vTaskDelayUntil(&CurrentTime2,1);

  }
  /* USER CODE END Encoder_Task */
}

/* USER CODE BEGIN Header_KeyMood_Task */
/**
* @brief Function implementing the KeyMoodTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_KeyMood_Task */
void KeyMood_Task(void *argument)
{
  /* USER CODE BEGIN KeyMood_Task */
  /* Infinite loop */
 portTickType CurrentTime3;
  for (;;)
  {
    CurrentTime3=xTaskGetTickCount();
    //usart_printf("4\n");
    keymood_task();
    vTaskDelayUntil(&CurrentTime3,1);

  }
  /* USER CODE END KeyMood_Task */
}

/* USER CODE BEGIN Header_Motor_Task */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Motor_Task */
void Motor_Task(void *argument)
{
  /* USER CODE BEGIN Motor_Task */
  /* Infinite loop */
  portTickType CurrentTime4;
  for (;;)
  {
    CurrentTime4=xTaskGetTickCount();
   // usart_printf("3\n");
    motor_task();
    vTaskDelayUntil(&CurrentTime4,1);
  }

  /* USER CODE END Motor_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

//任务测试，出现了操作系统里的任务和无法打印的问题
/*void Pid_Task(void *argument)
{
  portTickType CurrentTime;

  for (;;)
  {

    CurrentTime=xTaskGetTickCount();
    pid_task();
    vTaskDelayUntil(&CurrentTime,5);

  }


}

void Encoder_Task(void *argument)
{

  portTickType CurrentTime;

  for (;;)
  {
    CurrentTime=xTaskGetTickCount();
    encorder_task();
    vTaskDelayUntil(&CurrentTime,1);

  }
}

void Motor_Task(void *argument)
{

  portTickType CurrentTime;

  for (;;)
  {
    CurrentTime=xTaskGetTickCount();
    motor_task();
    vTaskDelayUntil(&CurrentTime,1);

  }
}

void KeyMood_Task(void *argument)
{
  portTickType CurrentTime;

  for (;;)
  {
    CurrentTime=xTaskGetTickCount();
    keymood_task();
    vTaskDelayUntil(&CurrentTime,1);

  }

}
*/
/* USER CODE END Application */

