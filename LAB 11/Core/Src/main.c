/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//control byte format
#define EEPROM_ADDR 0b10100000
#define IOEXPD_ADDR 0b01000000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t eepromExampleWriteFlag = 0;
uint8_t eepromExampleReadFlag = 0;
uint8_t IOExpdrExampleWriteFlag = 0;
uint8_t IOExpdrExampleReadFlag = 0;

uint8_t eepromDataReadBack;
uint8_t IOExpdrDataReadBack;
uint8_t IOExpdrDataWrite;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void EEPROMWriteExample();
void EEPROMReadExample(uint8_t *Rdata, uint16_t len);

void IOExpenderInit();
void IOExpenderReadPinA(uint8_t *Rdata);
void IOExpenderWritePinB(uint8_t Wdata);
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(100);
  IOExpenderInit();
  GPIO_PinState SwitchStateBB[2]; 	// 2 states => present,last	//Blue Button

  eepromExampleReadFlag = 1;
  HAL_Delay(10);

  EEPROMReadExample(&eepromDataReadBack, 1);
  HAL_Delay(100);

  IOExpdrExampleWriteFlag = 1;
  IOExpenderWritePinB(eepromDataReadBack);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		SwitchStateBB[0] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);		//BlueButton >> PC13
		if ( SwitchStateBB[1] == GPIO_PIN_SET && SwitchStateBB[0] == GPIO_PIN_RESET)		//Check pressing
		{
			IOExpdrExampleReadFlag = 1;
			IOExpdrExampleWriteFlag = 1;
			IOExpenderReadPinA(&IOExpdrDataReadBack);
			HAL_Delay(10);
			// LED 2 broken >> use LED 5-8 instead >> swap wire
			if (IOExpdrDataReadBack == 0b11110111)			//switch 1 LED 1
			{
				IOExpdrDataWrite = 0b01110000;
			}
			if (IOExpdrDataReadBack == 0b11111011)			//switch 2 LED 2
			{
				IOExpdrDataWrite = 0b10110000;
			}
			if (IOExpdrDataReadBack == 0b11111101)			//switch 3 LED 3
			{
				IOExpdrDataWrite = 0b11010000;
			}
			if (IOExpdrDataReadBack == 0b11111110)			//switch 4 LED 4
			{
				IOExpdrDataWrite = 0b11100000;
			}
			if (IOExpdrDataReadBack == 0b11110011)
			{
				IOExpdrDataWrite = 0b00110000;
			}
			if (IOExpdrDataReadBack == 0b11111001)
			{
				IOExpdrDataWrite = 0b10010000;
			}
			if (IOExpdrDataReadBack == 0b11110001)
			{
				IOExpdrDataWrite = 0b00010000;
			}
			if (IOExpdrDataReadBack == 0b11110101)
			{
				IOExpdrDataWrite = 0b01010000;
			}
			if (IOExpdrDataReadBack == 0b11110110)
			{
				IOExpdrDataWrite = 0b01100000;
			}
			if (IOExpdrDataReadBack == 0b11110000)
			{
				IOExpdrDataWrite = 0b00000000;
			}
			if (IOExpdrDataReadBack == 0b11110010)
			{
				IOExpdrDataWrite = 0b00100000;
			}
			if (IOExpdrDataReadBack == 0b11110100)
			{
				IOExpdrDataWrite = 0b01000000;
			}
			if (IOExpdrDataReadBack == 0b11111000)
			{
				IOExpdrDataWrite = 0b10000000;
			}
			if (IOExpdrDataReadBack == 0b11111010)
			{
				IOExpdrDataWrite = 0b10100000;
			}
			if (IOExpdrDataReadBack == 0b11111100)
			{
				IOExpdrDataWrite = 0b11000000;
			}
			if (IOExpdrDataReadBack == 0b11111111)
			{
				IOExpdrDataWrite = 0b11110000;
			}
			eepromDataReadBack = IOExpdrDataWrite;
			IOExpenderWritePinB(eepromDataReadBack);
			eepromExampleWriteFlag = 1;
			HAL_Delay(10);
			EEPROMWriteExample();
		}

		SwitchStateBB[1] = SwitchStateBB[0];
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void EEPROMWriteExample() {
	if (eepromExampleWriteFlag && hi2c1.State == HAL_I2C_STATE_READY)
	{
		static uint8_t data[1] = {0xff};
		data[0] = IOExpdrDataWrite;
		HAL_I2C_Mem_Write_IT(&hi2c1, EEPROM_ADDR, 0x03, I2C_MEMADD_SIZE_16BIT, data, 1);
		eepromExampleWriteFlag = 0;
	}
}
void EEPROMReadExample(uint8_t *Rdata, uint16_t len) {
	if (eepromExampleReadFlag && hi2c1.State == HAL_I2C_STATE_READY)
	{
		HAL_I2C_Mem_Read_IT(&hi2c1, EEPROM_ADDR, 0x03, I2C_MEMADD_SIZE_16BIT, Rdata, len);
		eepromExampleReadFlag = 0;
	}
}
void IOExpenderInit() {
	//Init All
	static uint8_t Setting[0x16] = { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00 };
	HAL_I2C_Mem_Write(&hi2c1, IOEXPD_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, Setting, 0x16, 100);
}

void IOExpenderReadPinA(uint8_t *Rdata) {
	if (IOExpdrExampleReadFlag && hi2c1.State == HAL_I2C_STATE_READY)
	{
		HAL_I2C_Mem_Read_IT(&hi2c1, IOEXPD_ADDR, 0x12, I2C_MEMADD_SIZE_8BIT, Rdata, 1);
		IOExpdrExampleReadFlag =0;
	}
}
void IOExpenderWritePinB(uint8_t Wdata) {
	if (IOExpdrExampleWriteFlag && hi2c1.State == HAL_I2C_STATE_READY)
	{
		static uint8_t data;
		data = Wdata;
		HAL_I2C_Mem_Write_IT(&hi2c1, IOEXPD_ADDR, 0x15, I2C_MEMADD_SIZE_8BIT, &data, 1);
		IOExpdrExampleWriteFlag=0;
	}
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
