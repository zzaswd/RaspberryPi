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
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

 ETH_HandleTypeDef heth;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char *pArray[10] = {0};
char * pToken;

void delay_us(uint16_t delay)
{
	TIM3->CNT =0;
	while(TIM3->CNT < delay );
}

int getKeyNumber()
{
	int scanValue = 0;

	for(int i = 0; i<4; i++ ){
		uint16_t out_pin= 0x1 << (8+i);
		HAL_GPIO_WritePin(GPIOC, out_pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, ~(out_pin), GPIO_PIN_SET);

		HAL_Delay(30);

		uint8_t in_pin = 0x10;
		if(HAL_GPIO_ReadPin(GPIOD,in_pin) == GPIO_PIN_RESET) {scanValue = i+1; break;}
		else if(HAL_GPIO_ReadPin(GPIOD,in_pin<<1) == GPIO_PIN_RESET) {scanValue = i+5; break;}
		else if(HAL_GPIO_ReadPin(GPIOD,in_pin<<2) == GPIO_PIN_RESET) {scanValue = i+9; break;}
		else if(HAL_GPIO_ReadPin(GPIOD,in_pin<<3) == GPIO_PIN_RESET) {scanValue = i+13; break;}

	}
	HAL_Delay(300);
	return scanValue;
}

int WriteString(int num, char* id, char * pass,int idx,int flag){
	if(flag == 0){	// ID
		if(num == 1) { id[idx] = 'a'; return flag;}
		else if(num == 2) { id[idx] = 'b'; return flag;}
		else if(num == 3) { id[idx] = 'c'; return flag;}
		else if(num == 4) { id[idx] = ' '; return -1;}
		else if(num == 5) { id[idx] = 'd'; return flag;}
		else if(num == 6) { id[idx] = 'e'; return flag;}
		else if(num == 7) { id[idx] = 'f'; return flag;}
		else if(num == 8) { id[idx] = 'g'; return flag;}
		else if(num == 9) { id[idx] = '1'; return flag;}
		else if(num == 10) { id[idx] = '2'; return flag;}
		else if(num == 11) { id[idx] = '3'; return flag;}
		else if(num == 12) { id[idx] = '4'; return flag;}
		else if(num == 13) { id[idx] = '5'; return flag;}
		else if(num == 14) { id[idx] = '6'; return flag;}
		else if(num == 15) { id[idx] = '7'; return flag;}
		else if(num == 16) { id[idx] = '\0'; return 1;}
	}
	else {			// password
		if(num == 1) { pass[idx] = 'a'; return 3;}
		else if(num == 2) { pass[idx] = 'b'; return 3;}
		else if(num == 3) { pass[idx] = 'c'; return 3;}
		else if(num == 4) { pass[idx] = ' '; return -1;}
		else if(num == 5) { pass[idx] = 'd'; return 3;}
		else if(num == 6) { pass[idx] = 'e'; return 3;}
		else if(num == 7) { pass[idx] = 'f'; return 3;}
		else if(num == 8) { pass[idx] = 'g'; return 3;}
		else if(num == 9) { pass[idx] = '1'; return 3;}
		else if(num == 10) { pass[idx] = '2'; return 3;}
		else if(num == 11) { pass[idx] = '3'; return 3;}
		else if(num == 12) { pass[idx] = '4'; return 3;}
		else if(num == 13) { pass[idx] = '5'; return 3;}
		else if(num == 14) { pass[idx] = '6'; return 3;}
		else if(num == 15) { pass[idx] = '7'; return 3;}
		else if(num == 16) { pass[idx] = '\0'; return 2;}
	}
	return 10;
}

void Missile()
{
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay(3000);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_RESET);
}

void LCDMain(void) // Main LCD
{
	lcd_clear();
	lcd_put_cur(0, 4);
	lcd_send_string("WELCOME!");
	lcd_put_cur(1, 0);
	lcd_send_string("8:C 12:L 16:R");
}
void LCD_LogIn(char*ID, char* PASS)
{
	lcd_clear();
	lcd_put_cur(0, 0);	lcd_send_string("ID : ");	lcd_send_string(ID);
	lcd_put_cur(1, 0);	lcd_send_string("PW : ");	lcd_send_string(PASS);
}

// LCD 로그?�� ?���??????? ?�� ?���???????
void LCD_LogInTrue(void)
{
	lcd_clear();
	lcd_put_cur(0, 4);	lcd_send_string("SUCCESS!");
	lcd_put_cur(1, 3);	lcd_send_string("DOOR OPEN");
	TIM3->CCR1 = 2000;	HAL_Delay(2000);
	TIM3->CCR1 = 1000;	HAL_Delay(2000);
}

void LCD_LogInFalse_Missile(void)
{
	lcd_clear();
	lcd_put_cur(0, 4);	lcd_send_string("FAILURE!");
	lcd_put_cur(1, 1);	lcd_send_string("Missile Launch");
	Missile();
}

void LCD_LogInFalse(void)
{
	lcd_clear();
	lcd_put_cur(0, 4);	lcd_send_string("FAILURE!");
	lcd_put_cur(1, 1);	lcd_send_string("Please Retry..");
	HAL_Delay(2000);
}

void LCD_User_data(char** ID,int line){
	lcd_clear();
	lcd_put_cur(0, 0);
	lcd_send_string("ID :");	lcd_send_string(ID[line]);
	lcd_put_cur(1, 0);
	lcd_send_string("ID :");	lcd_send_string(ID[line+1]);
}


void LCD_Register_Success(void) // ID, PW
{
	lcd_clear();
	lcd_put_cur(0, 3);	lcd_send_string("Register");
	lcd_put_cur(1, 3);	lcd_send_string("Success!!");
	HAL_Delay(2000);
}

void RegisterData(void){

	char id[20] = "";
	char pass[20]= "";
	int flag = 0;
	int idx = 0;

	while(1){
		int pressedKey = getKeyNumber();
		int result = WriteString(pressedKey, id, pass, idx, flag);
		LCD_LogIn(id,pass);
		if(result == -1){
			if(idx!=0){
				idx--;
				if(flag == 0) id[idx]= ' ';
				else pass[idx] = ' ';
			}
		}
		else if(result == 1){
			flag = 1; idx =0;
		}
		else if(result == 2){
			char temp[40];
			sprintf(temp,"100:%s:%sL",id,pass);
			HAL_UART_Transmit(&huart3, temp, strlen(temp), 20);
			LCD_Register_Success();
			break;
		}
		else if(result == 10) continue;
		else idx++;
	}
	HAL_Delay(300);
}

void ID_Check(void){
/*

	char temp[10];
	sprintf(temp,"%dL",200);
	HAL_UART_Transmit(&huart3, temp, strlen(temp), 10);
*/
	HAL_UART_Transmit(&huart3, "200L", strlen("200L"), 10);

	uint8_t RxBuffer[100];
	uint8_t Rx_temp;
	int str_len;
	char* ID[30]={0};
	int idx = 0;
	delay_us(10);
	while(1){
		if(HAL_UART_Receive(&huart3, &Rx_temp, 1, 10)==HAL_OK){
			RxBuffer[idx++] = Rx_temp;
			delay_us(10);
			if(Rx_temp == 'L') break;
		}
	}
	RxBuffer[idx-1] = '\0';
	//str_len = strlen(RxBuffer);

	pToken = strtok(RxBuffer,":");
	//int i = 0;
	idx = 0;
	while(pToken !=NULL){
		ID[idx] = pToken;
		if(++idx>10)	break;
		pToken = strtok(NULL,":");
	}

	int line = 0;
	while(1){
		LCD_User_data(ID, line);
		if(getKeyNumber()==1){ //up
			if(line !=0) line--;
		}
		else if(getKeyNumber()==2){
			if(line<idx-1) line ++;
		}
		else if(getKeyNumber()==4) break;
	}
}

void LogIn(void){
	char id[20] = "";
	char pass[20] = "";
	int flag = 0;
	int idx = 0;
	int count = 0;
	while(1){
		int pressedKey = getKeyNumber();
		int result = WriteString(pressedKey, id, pass, idx, flag);
		LCD_LogIn(id,pass);
		if(result == -1){
			if(idx!=0){
				idx--;
				if(flag == 0) id[idx]= ' ';
				else pass[idx] = ' ';
			}
		}
		else if(result == 1){
			flag = 1;
			idx =0;
		}
		else if(result == 2){
			char temp[40];
			sprintf(temp,"300:%s:%sL",id,pass);
			HAL_UART_Transmit(&huart3, temp, strlen(temp), 20);

			delay_us(10);
			char exist = 0;
			while(1){
				if(HAL_UART_Receive(&huart3, &exist, 1, 10)==HAL_OK)	break;
			}

			if(exist == '0'){
				if(count ==2) {
					HAL_UART_Transmit(&huart3,"400L",strlen("400L"),20);
					LCD_LogInFalse_Missile();
					break;
				}
				else{
					LCD_LogInFalse();
					flag = 0;
					idx = 0;
					sprintf(id," ");
					sprintf(pass," ");
					count++;
				}
				HAL_Delay(500);
			}
			else if(exist == '1'){
				LCD_LogInTrue();
				break;
			}
		}
		else if(result == 10) continue;
		else idx++;
	}
}


void Camera_Control(void){

	uint8_t RxBuffer[10];
	uint8_t Rx_temp;
	int idx = 0;
	HAL_UART_Transmit(&huart3, "CL", strlen("CL"), 20);

	delay_us(10);

	while(1){
		if(HAL_UART_Receive(&huart3, &Rx_temp, 1, 10)==HAL_OK){
			if(Rx_temp == 'C') break;
			RxBuffer[idx++] = Rx_temp;
			delay_us(10);
			if(Rx_temp == 'L') break;
		}
	}
	RxBuffer[idx-1] = '\0';

	int angle = atoi(RxBuffer);

	TIM3->CCR2 = angle*10; // 60~240
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
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  int pressedKey;
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_RESET);
  lcd_init();
  //LCDMain();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // 4 : ?��?�� �??????????,
  // 8 :
  // 12 :
  // 16 :  ?��?��

  while (1)
  {
	  pressedKey = getKeyNumber();


	  if(pressedKey == 16)  RegisterData();

	  else if(pressedKey == 12) ID_Check();

	  else if(pressedKey == 8) LogIn();

	  else  LCDMain();
	  Camera_Control();

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

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
  hi2c1.Init.ClockSpeed = 100000;
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

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 84-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PF3 PF5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD10 PD11 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 PC10 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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
