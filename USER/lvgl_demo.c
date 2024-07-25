/**
 ****************************************************************************************************
 * @file        lvgl_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-11
 * @brief       LVGL V8����ϵͳ��ֲ
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     1           /* �������ȼ� */
#define START_STK_SIZE      256         /* �����ջ��С */
TaskHandle_t StartTask_Handler;         /* ������ */
void start_task(void *pvParameters);    /* ������ */



/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO      3          /* �������ȼ� */
#define LED_STK_SIZE        64         /* �����ջ��С */
TaskHandle_t LEDTask_Handler;           /* ������ */
void led_task(void *pvParameters);      /* ������ */


/* VIDEO_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define VIDEO_TASK_PRIO       3          /* �������ȼ� */
#define VIDEO_STK_SIZE        256         /* �����ջ��С */
TaskHandle_t VIDEOTask_Handler;           /* ������ */
void video_task(void *pvParameters);      /* ������ */


/* AUDIO_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define AUDIO_TASK_PRIO       3          /* �������ȼ� */
#define AUDIO_STK_SIZE        256         /* �����ջ��С */
TaskHandle_t AUDIOTask_Handler;           /* ������ */
void audio_task(void *pvParameters);      /* ������ */
uint8_t keyVal = 0;				//��ֵ




/******************************************************************************************************/

/**
 * @brief       lvgl_demo��ں���
 * @param       ��
 * @retval      ��
 */
void lvgl_demo(void)
{
    printf("1111111111111111111");
    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ���ݸ��������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */
     
    vTaskStartScheduler();                              /* ����������� */
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    taskENTER_CRITICAL();           /* �����ٽ��� */


    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);
								
			 /* VIDEO�������� */
    xTaskCreate((TaskFunction_t )video_task,
                (const char*    )"video_task",
                (uint16_t       )VIDEO_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )VIDEO_TASK_PRIO,
                (TaskHandle_t*  )&VIDEOTask_Handler);
								
			 /* AUDIO�������� */
    xTaskCreate((TaskFunction_t )audio_task,
                (const char*    )"audio_task",
                (uint16_t       )AUDIO_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )AUDIO_TASK_PRIO,
                (TaskHandle_t*  )&AUDIOTask_Handler);					
								
								
								
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */						
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
						
}



/**
 * @brief       led_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void audio_task(void *pvParameters)
{
    pvParameters = pvParameters;
	

    while(1)
    { 
			if(keyVal == KEY0_PRES)	 
			{
				vTaskSuspend(VIDEOTask_Handler);         //����video_play();
			  audio_play();
				vTaskDelay(10);
				if(keyVal == KEY0_PRES)       //�ٴΰ���key0
				{
					vTaskSuspend(AUDIOTask_Handler);          //����audio����
					vTaskResume(VIDEOTask_Handler);           //�ָ�video����
					vTaskDelay(10);
				}
				
      }
			
	 }
}
