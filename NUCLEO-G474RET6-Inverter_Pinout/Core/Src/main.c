/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "mylibs/shell.h"
#include "mylibs/pwm.h"
#include "mylibs/codeur.h"
#include "mylibs/PID.h"

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

/* USER CODE BEGIN PV */

//Variables ADC
uint32_t buffer[ADC_BUF_SIZE]; //buffer de l'ADC
uint8_t flag=0;				   //flag de l'ADC en DMA
float Imes[3]={0};		       //valeurs de courant pour les 3 derniers instants

//Variables asservissement en courant
extern float Iconsigne[3];
extern float erreur_I[3];
extern float alpha[3];
h_PID_t *  h_PI_I;

//Variables asservissementr en vitesse
extern int consigne;
extern float erreur[3];
extern float vitesse[3];
h_PID_t	*  h_PI;

extern uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	int nb_loop = 0;
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
	MX_DMA_Init();
	MX_ADC2_Init();
	MX_ADC1_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_TIM16_Init();

	/* USER CODE BEGIN 2 */

	//On start le DMA
	if(HAL_OK != HAL_ADC_Start_DMA(&hadc1, buffer, ADC_BUF_SIZE)){
		Error_Handler();
	}

	//On start le codeur et le timer de vitesse
	codeur_start();

	//PI Speed
	h_PI->b0=0.48;
	h_PI->b1=0.247;
	h_PI->b2=-0.23;

	//PI Current
	h_PI_I->b0=0.9325;
	h_PI_I->b1=0.0001235;
	h_PI_I->b2=-0.73;

	//Démarrage des timers pour les PWM
	pwm_start();

	//Lancement du Shell
	Shell_Init();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		Shell_Loop();

		if(flag){
			nb_loop = (nb_loop + 1)%20000;
			if(nb_loop == 0){
				int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Valeur : %4d \r\n", Imes);
				HAL_UART_Transmit(&huart2, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
			}
			flag = 0;
		}

		//		HAL_ADC_Start(&hadc1);
		//		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		//		int value = HAL_ADC_GetValue(&hadc1);
		//		int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Valeur : %4d \r\n", value);
		//		HAL_UART_Transmit(&huart2, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);

		//		HAL_Delay(1000);

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

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/*
 * @brief
 * Ce Callback est appelé à chaque nouvelle mesure de courant
 * Il est déclenché par le timer 1 donc à une fréquence d'environ 20kHz
 * Ici on réalise l'asservissement en courant
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	//	if(HAL_ADC_Stop_DMA(&hadc1)!= HAL_OK){
	//		Error_Handler();
	//		flag=1;
	//	}
	//Interupt Mode
	//	adc_vall = HAL_ADC_GetValue(&hadc1);
	//	HAL_ADC_Start_IT(&hadc1);

	Imes[0]=Imes[1];
	Imes[1]=Imes[2];
	Imes[2]= (buffer[0]-1351)/40.95; //Nouvelle valeur de courant

	//Calcul de l'erreur en courant
	Erreur_I(Iconsigne,Imes);

	//Calcul de la nouvelle valeur de alpha après le PI
	Current_PI(h_PI_I,erreur_I,alpha);

	//Application des nouvelles commandes PWM
	NewPWM(alpha);

	flag = 1;
}
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */
	/*@brief
	 * Callback appelé toutes les 100ms lors de la mesure de la vitesse
	 * Ici on réalise l'asservissement en vitesse
	 */
	if (htim->Instance == TIM16){
		//Calcul de la vitesse de rotation du moteur
		calc_speed();

		//Calcul de l'erreur en vitesse
		Erreur(consigne, erreur, vitesse);

		//Calcul de la nouvelle consigne en courant grâce au PI
		Speed_PI(h_PI,erreur,Iconsigne);
	}

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

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
