/**
 ****************************************************************************************************
 * @file        lvgl_demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-11
 * @brief       LVGL V8操作系统移植
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#include "lvgl_demo.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "videoplayer.h"
#include "audioplay.h"
#include "key.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO     1           /* 任务优先级 */
#define START_STK_SIZE      256         /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;         /* 任务句柄 */
void start_task(void *pvParameters);    /* 任务函数 */



/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO      3          /* 任务优先级 */
#define LED_STK_SIZE        64         /* 任务堆栈大小 */
TaskHandle_t LEDTask_Handler;           /* 任务句柄 */
void led_task(void *pvParameters);      /* 任务函数 */


/* VIDEO_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define VIDEO_TASK_PRIO       3          /* 任务优先级 */
#define VIDEO_STK_SIZE        256         /* 任务堆栈大小 */
TaskHandle_t VIDEOTask_Handler;           /* 任务句柄 */
void video_task(void *pvParameters);      /* 任务函数 */


/* AUDIO_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define AUDIO_TASK_PRIO       3          /* 任务优先级 */
#define AUDIO_STK_SIZE        256         /* 任务堆栈大小 */
TaskHandle_t AUDIOTask_Handler;           /* 任务句柄 */
void audio_task(void *pvParameters);      /* 任务函数 */
uint8_t keyVal = 0;				//键值




/******************************************************************************************************/

/**
 * @brief       lvgl_demo入口函数
 * @param       无
 * @retval      无
 */
void lvgl_demo(void)
{
    printf("1111111111111111111");
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传递给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
     
    vTaskStartScheduler();                              /* 开启任务调度 */
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    taskENTER_CRITICAL();           /* 进入临界区 */


    /* LED测试任务 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);
								
			 /* VIDEO测试任务 */
    xTaskCreate((TaskFunction_t )video_task,
                (const char*    )"video_task",
                (uint16_t       )VIDEO_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )VIDEO_TASK_PRIO,
                (TaskHandle_t*  )&VIDEOTask_Handler);
								
			 /* AUDIO测试任务 */
    xTaskCreate((TaskFunction_t )audio_task,
                (const char*    )"audio_task",
                (uint16_t       )AUDIO_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )AUDIO_TASK_PRIO,
                (TaskHandle_t*  )&AUDIOTask_Handler);					
								
								
								
    taskEXIT_CRITICAL();            /* 退出临界区 */						
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
						
}



/**
 * @brief       led_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;
    while(1)
    {
      GPIO_ResetBits(GPIOF, GPIO_Pin_9);
			vTaskDelay(100);
			GPIO_SetBits(GPIOF, GPIO_Pin_9);
			vTaskDelay(100);
    }
}
/**
 * @brief       video_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void video_task(void *pvParameters)
{
    pvParameters = pvParameters;
    while(1)
    {

			video_play();
			vTaskDelay(100);
    }
}

/**
 * @brief       audio_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void audio_task(void *pvParameters)
{
    pvParameters = pvParameters;
	

    while(1)
    { 
			if(keyVal == KEY0_PRES)	 
			{
				vTaskSuspend(VIDEOTask_Handler);         //挂起video_play();
			  audio_play();
				vTaskDelay(10);
				if(keyVal == KEY0_PRES)       //再次按下key0
				{
					vTaskSuspend(AUDIOTask_Handler);          //挂起audio任务
					vTaskResume(VIDEOTask_Handler);           //恢复video任务
					vTaskDelay(10);
				}
				
      }
			
	 }
}
