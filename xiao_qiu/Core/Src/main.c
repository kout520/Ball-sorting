/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "oled.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RECEIVE_BUFFER_SIZE 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t data = 0;
uint8_t SSS[RECEIVE_BUFFER_SIZE];

int count = 0;
int abc = 0;
int bcd = 0;
int cde = 0;
int pink = 0;
int qing = 0;
int orgic = 0;

char buf[10] = {0};
char aaa[10] = {0};
char bbb[10] = {0};
char ccc[10] = {0};
char ddd[10] = {0};
char eee[10] = {0};
char fff[10] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if (SSS[0] != data) {
	        data = SSS[0];  // 更新上一次接收到的数据

	        // 根据接收到的数据更新OLED上的小球计数
	        switch (SSS[0]) {  case '6': pink++; __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 100); break;//粉色
	                           case '2': count++; __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 120); break;//蓝色
	      	                   case '3': abc++; __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 145); break;//红色
	      	                   case '4': bcd++; __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 168); break;//绿色
	      	                   case '5': cde++; __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 195); break;//黄色
	      	                 case '7': qing++; __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 214); break;//青色
	      	               case '8': orgic++; __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 229); break;//橙色
	    }
	}

    HAL_UART_Receive_IT(&huart2, SSS, RECEIVE_BUFFER_SIZE);
}
void updateDisplay() {
	    sprintf(buf, "%d ", count);
	    sprintf(aaa, "%d ", abc);
	    sprintf(bbb, "%d ", bcd);
	    sprintf(ccc, "%d ", cde);
	    sprintf(ddd, "%d ", pink);
	    sprintf(eee, "%d ", qing);
	    sprintf(fff, "%d ", orgic);
	    OLED_PrintString(16, 0, buf, &font16x16, OLED_COLOR_NORMAL);
	    OLED_PrintString(16, 16, aaa, &font16x16, OLED_COLOR_NORMAL);
	    OLED_PrintString(16, 32, bbb, &font16x16, OLED_COLOR_NORMAL);
	    OLED_PrintString(16, 48, ccc, &font16x16, OLED_COLOR_NORMAL);
	    OLED_PrintString(48, 0, ddd, &font16x16, OLED_COLOR_NORMAL);
	    OLED_PrintString(48, 16, eee, &font16x16, OLED_COLOR_NORMAL);
	    OLED_PrintString(48, 32, fff, &font16x16, OLED_COLOR_NORMAL);
	    OLED_ShowFrame();
	}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, SSS, RECEIVE_BUFFER_SIZE);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

    HAL_Delay(100); // 单片机启动比OLED上电�??,�??要延迟等�??
    OLED_Init();

    OLED_NewFrame();
    OLED_PrintString(0, 0, "蓝", &font16x16, OLED_COLOR_NORMAL);
        OLED_PrintString(0, 16, "红", &font16x16, OLED_COLOR_NORMAL);
        OLED_PrintString(0, 32, "绿", &font16x16, OLED_COLOR_NORMAL);
        OLED_PrintString(0, 48, "黄", &font16x16, OLED_COLOR_NORMAL);
        OLED_PrintString(32, 0, "粉", &font16x16, OLED_COLOR_NORMAL);
        OLED_PrintString(32, 16, "浅", &font16x16, OLED_COLOR_NORMAL);
        OLED_PrintString(32, 32, "橙", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while(1){
    	updateDisplay();
    	 // 舵机控制占空比范围2.5% ~ 12.5%
    	    // Counter Period设置的是2000，因此占空比设置范围是50 ~ 250

    	    // 中点，占空比7.5%，即2000 * 7.5% = 150
    	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 170);

    	    HAL_Delay(1500);
    	    // 向左转，占空比2.5%，即2000 * 2.5% = 50
    	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 120);
    	    HAL_Delay(1500);

    	    // 向右转，占空比12.5%，即2000 * 12.5% = 250
    	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 50);
    	    HAL_Delay(2000);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
