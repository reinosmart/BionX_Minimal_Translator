/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include <stdio.h>
#include "config.h"
#include "display_kunteng.h"
#include "CAN-Registers.h"
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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CAN_HandleTypeDef hcan;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
char UART_TX_Buffer[100];
char UART_RX_Buffer[100];
uint8_t i=0; //counter for loops
uint8_t j=0; //counter for autozero at startup
uint8_t k=0; //counter for fast CAN_TX
uint8_t l=0; //counter for slow CAN_TX
uint8_t UART_RX_Flag=0;
uint8_t Brake_Flag_Old=0;
uint8_t UART_TX_Flag=1;
uint8_t CAN_RX_Flag=0;
uint8_t CAN_TX_Flag=0;
uint8_t Timer3_Flag=0;
uint8_t ADC_Flag=0;
uint8_t PAS_Flag=0;
char ch_GaugeVoltage_Lo=0;
char ch_GaugeVoltage_Hi=0;
char ch_BatteryVoltage_Lo=0;
char ch_BatteryVoltage_Hi=0;

int8_t i8_Throttle=0; //must be scaled to valid values from -64 ... +64
int16_t i16_Gauge_Torque=0;
int16_t i16_Gauge_Voltage=0;
uint16_t ui16_Ext_Torque=0;
uint32_t ui32_Ext_Torque_Cumulated=0;
int16_t i16_PAS_Counter=0;
int16_t i16_PAS_Duration=PAS_TIMEOUT;
int32_t i32_Gauge_Torque_cumulated=0;
int16_t i16_Current_Target=0;
uint16_t ui16_slow_loop_counter=0;
uint16_t ui16_Gauge_Gain;
volatile uint16_t adcData[4]; //Buffer for ADC1 Input
MotorState_t MS; //struct for motor state


CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CAN_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);


int32_t map (int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
int16_t PI_control (int16_t ist, int16_t soll, int16_t max);
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
  MX_CAN_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
#if (DISPLAY_TYPE == DISPLAY_TYPE_KUNTENG)
  kunteng_init();
#endif
  /* Run the ADC CALIB_GAUGEration */


  {
    /* CALIB_GAUGEration Error */
    Error_Handler();
  }

  MX_TIM3_Init();

  // Start Timer 3

  if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
       {
         /* Counter Enable Error */
         Error_Handler();
       }
  // Start ADC1 with DMA
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcData, 4);

  /* USER CODE BEGIN 2 */

  UART_RX_Buffer[0]=0;


  while(!CAN_RX_Flag){ //So lange Versionsanfrage senden, bis Antwort vom BionX-Controller kommt, dabei blinken.
  	  HAL_Delay(200);
  	  HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
		Send_CAN_Request(REG_MOTOR_REV_SW);

		  }
  HAL_Delay(200);

  Send_CAN_Command(REG_MOTOR_PROTECT_UNLOCK,MOTOR_PROTECT_UNLOCK_KEY);
  Send_CAN_Command(REG_MOTOR_TORQUE_GAUGE_GAIN,30); // 30=neuer Gauge_Gain
  ui16_Gauge_Gain=0;
  Send_CAN_Request(REG_MOTOR_TORQUE_GAUGE_GAIN); // Gauge_Gain abfragen, nur zur Kontrolle
  ui16_Gauge_Gain=RxData[3];

  MS.Gauge_Ext_Torq_Flag = 1; 	//set torque source to external BB-sensor by default. Is overwritten by KT-LCD setting of P3 at runtime.
  MS.Assist_Level = 1;			//set Assistlevel to 3 as default value
  MS.Gauge_Factor = 127;		//default for global gain for torque measurement, set by Kunteng Paramter P1 0 ... 255
  MS.Regen_Factor = 2;			//default regen strenght for brake lever regen
  MS.Throttle_Function = 0; 	//Throttle override for power and regen
  MS.Min_Voltage = 20000;		//minimum Voltage (mV) for 10s pack as default
  MS.Max_Voltage = 46000;		//maximum Voltage (mV) for 10s pack as default

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



	  if(UART_RX_Flag){
		  UART_RX_Flag=0;

#if (DISPLAY_TYPE == DISPLAY_TYPE_DEBUG)
		  if(UART_TX_Flag){
			 UART_TX_Flag=0;


		  	sprintf(UART_TX_Buffer, "Empfangenes UART Byte %d, %d, %d, %d\r\n",UART_RX_Buffer[0],UART_RX_Buffer[1], UART_RX_Buffer[2], UART_RX_Buffer[3]);

		  	  	  i=0;
		  	  	  while (UART_TX_Buffer[i] != '\0')
		  	  	  {i++;}


		  	  	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&UART_TX_Buffer, i);

		  }

		  	  if(CAN_TX_Flag && UART_RX_Buffer[0]==1){
		  		CAN_TX_Flag=0;
		  		Send_CAN_Command(UART_RX_Buffer[1],((uint16_t)UART_RX_Buffer[2]<<8)+UART_RX_Buffer[3]); //send command with UART-Input
		  	  }


#endif
#if (DISPLAY_TYPE == DISPLAY_TYPE_KUNTENG)
		  	check_message(&MS);

#endif
	  } //end uart rx flag

	  //Timer 3 running with 1kHz ISR frequency
	  if(Timer3_Flag){

		  Timer3_Flag=0;
		  ui16_slow_loop_counter++;
		  if (i16_PAS_Counter<PAS_TIMEOUT)i16_PAS_Counter++;

		  MS.Voltage=ch_BatteryVoltage_Hi<<8|ch_BatteryVoltage_Hi; //Battery Voltage in mV
		  i16_Gauge_Voltage=ch_GaugeVoltage_Hi<<8|ch_GaugeVoltage_Lo;

		  //Bremseingang abfragen
		  MS.Brake=HAL_GPIO_ReadPin(Brake_GPIO_Port, Brake_Pin);
		  // hier ggf. noch entprellen?
		  if(MS.Brake==0 && Brake_Flag_Old==1){
			  PI_control (MS.Voltage, MS.Max_Voltage, 0); //reset integral part of PI control, if brake lever is released
			  if(MS.Assist_Level==0){ //Toggle
				  if(MS.Perma_Regen)MS.Perma_Regen=0;
				  else MS.Perma_Regen=1;
			  }
		  }
		  Brake_Flag_Old=MS.Brake;
		  //read in throttle signal
		  i8_Throttle=map(adcData[3],THROTTLE_MIN,THROTTLE_MAX, 0, 63); //map throttle ADC-value to valid LEVEL range

		  //process PAS signal
		  if (PAS_Flag){
			  HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
			  PAS_Flag=0;
			  i16_PAS_Duration=i16_PAS_Counter;
			  i16_PAS_Counter=0;
			  ui16_Ext_Torque=map(adcData[1],EXT_TORQUE_MIN, EXT_TORQUE_MAX, 0, 1024); //map throttle ADC-value to valid LEVEL range
			  ui32_Ext_Torque_Cumulated-=ui32_Ext_Torque_Cumulated>>FILTER;
			  ui32_Ext_Torque_Cumulated+=ui16_Ext_Torque;

		  }

		  if (ui16_slow_loop_counter>5){

			  ui16_slow_loop_counter=0;

			  //Brake acitve, do constant regen
			  if(!MS.Brake){
				  i16_Current_Target = (-CALIB_REGEN*(MS.Regen_Factor+1))>>3;		//regen via brake lever
				  if(!MS.Throttle_Function)i8_Throttle=-i8_Throttle;
			  }
			  //Perma Regen active, do regen according to assist level
			  else if(MS.Perma_Regen){
				  i16_Current_Target = (-CALIB_REGEN*MS.Assist_Level)/5;		//permanent regen switched by tip on brake lever in level 0
				  if(!MS.Throttle_Function)i8_Throttle=-i8_Throttle;
			  }
			  // if internal Gauge is active (selected by P3), calculate current target form Gauge Value,
			  else if(MS.Gauge_Ext_Torq_Flag)i16_Current_Target = (CALIB_GAUGE*(i32_Gauge_Torque_cumulated>>FILTER)*MS.Assist_Level*MS.Gauge_Factor)>>7; //normal ride mode
			  // if external torque sensor is active
			  else {
				  i16_Current_Target = (((ui32_Ext_Torque_Cumulated>>FILTER)*MS.Assist_Level*MS.Gauge_Factor)>>CALIB_EXT_TORQUE)/(i16_PAS_Duration*(MS.Speed+1));
				  if(i16_PAS_Counter>PAS_TIMEOUT-1){
					  i16_Current_Target=0; //Switch off power, if pedals are not turning
					  if(ui32_Ext_Torque_Cumulated>0)ui32_Ext_Torque_Cumulated--;
					  if(i16_PAS_Duration<PAS_TIMEOUT)i16_PAS_Duration++;

				  }
			  }

			  // limit Current_Target to valid range for LEVEL
			  if(i16_Current_Target>63)i16_Current_Target=63;
			  if(i16_Current_Target<-63)i16_Current_Target=-63;

			  if(!MS.Throttle_Function){
				  // Throttle override for regen and assist
				  if(i16_Current_Target>=0 && i8_Throttle>0 && i8_Throttle>i16_Current_Target)i16_Current_Target=i8_Throttle;
				  if(i16_Current_Target<=0 && i8_Throttle<0 && i8_Throttle<i16_Current_Target)i16_Current_Target=i8_Throttle;
			  }
			  else {//linear regen with throttle
				  if(i8_Throttle>2)i16_Current_Target = -i8_Throttle;
			  }

		//	  if (i16_Current_Target<0) i16_Current_Target= -map(MS.Voltage,MS.Max_Voltage-2000,MS.Max_Voltage,-i16_Current_Target,0);
			  if (i16_Current_Target<0) {
				  i16_Current_Target = PI_control (MS.Voltage, MS.Max_Voltage, i16_Current_Target);
			  }
			 /* if (ADC_Flag){
				  ADC_Flag=0;
				  sprintf(UART_TX_Buffer, "ADC Values %d, %d, %d\r\n",adcData[0], adcData[1], adcData[2]);

				  		  	  	  i=0;
				  		  	  	  while (UART_TX_Buffer[i] != '\0')
				  		  	  	  {i++;}

				  		  	  	HAL_UART_IRQHandler(&huart1);
				  		  	  	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&UART_TX_Buffer, i);
				  		  	  	HAL_Delay(200);

			  }*/

				 // Send next CAN_TX if last CAN transmit is finished
			  if(CAN_TX_Flag){

				  CAN_TX_Flag=0;

				  switch (k) {

				  case 0:
#if (DISPLAY_TYPE == DISPLAY_TYPE_DEBUG)
					  if(!UART_RX_Buffer[0]){

						  Send_CAN_Request(REG_MOTOR_TORQUE_GAUGE_VALUE);

					  }
					  else{

						  Send_CAN_Request(UART_RX_Buffer[1]);
					  }
#endif
#if (DISPLAY_TYPE == DISPLAY_TYPE_KUNTENG)

						  Send_CAN_Request(REG_MOTOR_TORQUE_GAUGE_VALUE);

#endif
					  k++;
				  break;

				  case 1:
				  // send current target to BionX controller, perhaps 2 times, perhaps wait for CAN TX ready.
#if (DISPLAY_TYPE == DISPLAY_TYPE_DEBUG)
					  if(!UART_RX_Buffer[0]){
						if(MS.Voltage>MS.Min_Voltage) Send_CAN_Command(REG_MOTOR_ASSIST_LEVEL,i16_Current_Target); //send current target only when battery voltage is high enough
						else CAN_TX_Flag = 1;
					  }
					  else{
						  Send_CAN_Request(REG_MOTOR_STATUS_TEMPERATURE);
					  }
#endif
#if (DISPLAY_TYPE == DISPLAY_TYPE_KUNTENG)

					  if(MS.Voltage>MS.Min_Voltage) Send_CAN_Command(REG_MOTOR_ASSIST_LEVEL,i16_Current_Target); //send current target only when battery voltage is high enough
					  else CAN_TX_Flag = 1;

#endif

					  k++;
				  break;



//slow CAN_TX
				  case 2:
					  switch (l) {

					  	  case 0:
					  		  Send_CAN_Request(REG_MOTOR_STATUS_POWER_METER);
							  l++;
						  break;

					  	  case 1:
					  		Send_CAN_Request(REG_MOTOR_STATUS_TEMPERATURE);
							  l++;
						  break;

						  case 2:
							  Send_CAN_Request(REG_MOTOR_STATUS_SPEED);
							  l++;
						  break;

						  case 3:
							  Send_CAN_Request(REG_MOTOR_STATUS_POWER_VOLTAGE_HI);
							  l++;
						  break;

						  case 4:
							  Send_CAN_Request(REG_MOTOR_STATUS_POWER_VOLTAGE_LO);
							  l=0;
						  break;
/*
						  case 5:
							  Send_CAN_Request(REG_MOTOR_TORQUE_GAUGE_VOLTAGE_LO);
							  l++;
						  break;

						  case 6:
							  Send_CAN_Request(REG_MOTOR_TORQUE_GAUGE_VOLTAGE_HI);
							  l++;
						  break;
*/


					  }//end switch of slow CAN_TX

					  k=0;
				  break;

				  }//end switch of fast CAN_TX
			  }//end TX_Flag

		  }//end slow loop
	  }// end timer 1 kHz loop



	  if(CAN_RX_Flag){

		  CAN_RX_Flag=0;



		  switch (RxData[1]) {

		  case REG_MOTOR_TORQUE_GAUGE_VALUE:
			  i16_Gauge_Torque=RxData[3];
			  i32_Gauge_Torque_cumulated -= (i32_Gauge_Torque_cumulated>>FILTER); // FILTER = 6 f�r 1s Zeitkonstante
			  i32_Gauge_Torque_cumulated += i16_Gauge_Torque;

			  break;

		  case REG_MOTOR_STATUS_SPEED:

			  MS.Speed=RxData[3];


			  break;

		  case REG_MOTOR_STATUS_POWER_METER:

			  MS.Power=RxData[3];


			  break;

		  case REG_MOTOR_TORQUE_GAUGE_VOLTAGE_HI:

			  ch_GaugeVoltage_Hi=RxData[3];


			  break;

		  case REG_MOTOR_TORQUE_GAUGE_VOLTAGE_LO:

			  ch_GaugeVoltage_Lo=RxData[3];


			  break;

		  case REG_MOTOR_STATUS_TEMPERATURE:

			  MS.MotorTemperature=RxData[3];

			  break;

		  case REG_MOTOR_STATUS_POWER_VOLTAGE_HI:

			  ch_BatteryVoltage_Hi=RxData[3];

			  break;

		  case REG_MOTOR_STATUS_POWER_VOLTAGE_LO:

			  ch_BatteryVoltage_Lo=RxData[3];

			  break;



		  }

#if (DISPLAY_TYPE == DISPLAY_TYPE_DEBUG)
		  //print out received CAN message
		  if(UART_TX_Flag){



		  if(!UART_RX_Buffer[0]){

			  sprintf(UART_TX_Buffer, "%d, %ld, %d, %d, %ld, %d, %d, %d, %d, %d, %d \r\n", MS.MotorTemperature, i32_Gauge_Torque_cumulated, MS.Power, MS.Speed, MS.Voltage, i16_PAS_Duration, i16_Current_Target, i16_Gauge_Torque, ui16_Gauge_Gain, adcData[2], adcData[3]);
			  i=0;
			  while (UART_TX_Buffer[i] != '\0')
			  {i++;}

			  UART_TX_Flag=0;
			  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&UART_TX_Buffer, i);
		  	  }
		  else {

			  sprintf(UART_TX_Buffer, "%d, %d, %d, %d, %d, %d, %d\r\n", (int16_t) RxHeader.StdId, (int16_t) RxHeader.IDE, (int16_t) RxHeader.DLC, RxData[0],RxData[1],RxData[2],RxData[3]);

			  i=0;
			  while (UART_TX_Buffer[i] != '\0')
			  {i++;}

			  UART_TX_Flag=0;
			  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&UART_TX_Buffer, i);
		  }


			  }
#endif
		  }//End CAN Rx


	  }


    /* USER CODE END WHILE */



    /* USER CODE BEGIN 3 */
  } //end while loop
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_4TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_5TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;


  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  /* USER CODE BEGIN CAN_Init 2 */
  /*##-2- Configure the CAN Filter ###########################################*/
  	CAN_FilterTypeDef  sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
    {
      /* Filter configuration Error */
      Error_Handler();
    }

    /*##-3- Start the CAN peripheral ###########################################*/
    if (HAL_CAN_Start(&hcan) != HAL_OK)
    {
      /* Start Error */
      Error_Handler();
    }

    /*##-4- Activate CAN RX notification #######################################*/
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
      /* Notification Error */
      Error_Handler();
    }

    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
    {
      /* Notification Error */
      Error_Handler();
    }

    /*##-5- Configure Transmission process #####################################*/
    TxHeader.StdId = ID_MOTOR;
    TxHeader.ExtId = 0x01;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 4;
    TxHeader.TransmitGlobalTime = DISABLE;
  /* USER CODE END CAN_Init 2 */

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

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 512;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 139;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
#if (DISPLAY_TYPE == DISPLAY_TYPE_DEBUG)
  huart1.Init.BaudRate = 115200;
#endif
#if (DISPLAY_TYPE == DISPLAY_TYPE_KUNTENG)
  huart1.Init.BaudRate = 9600;
#endif
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
#if (DISPLAY_TYPE == DISPLAY_TYPE_DEBUG)
  if (HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART_RX_Buffer, 4) != HAL_OK)
   {
	   Error_Handler();
   }

#endif
  /* USER CODE END USART1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
	  /* DMA controller clock enable */
	  __HAL_RCC_DMA1_CLK_ENABLE();

	  /* DMA interrupt init */
	  /* DMA1_Channel1_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	  /* DMA1_Channel4_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 1, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	  /* DMA1_Channel5_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 1, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

	  HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
	  HAL_NVIC_EnableIRQ(USART1_IRQn);


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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Onboard_LED_Pin */
  GPIO_InitStruct.Pin = Onboard_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Onboard_LED_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Light_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Light_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PAS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Brake_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
/* already defined in msp.c
  GPIO_InitStruct.Pin = BatteryVoltage_Pin|Torque_Pin|BatteryCurrent_Pin|Throttle_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  */

  //activate Interrupt for PAS Line
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/* USER CODE BEGIN 4 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

	UART_TX_Flag=1;
	//HAL_UART_IRQHandler(&huart1);// reset huart1

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UART_RX_Flag=1;


}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
  /* Get RX message */
	//HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
  if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }

  CAN_RX_Flag=1;

}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *CanHandle)
{


	CAN_TX_Flag=1;

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim3) {
		Timer3_Flag=1;


	}
}

// regular ADC callback
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	ADC_Flag=1;

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	PAS_Flag=1;

}

void Send_CAN_Request(uint8_t command){

	TxHeader.StdId=ID_MOTOR;
	TxHeader.DLC=2;
	TxData[0] = 0;
	TxData[1] = command;

	//HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	   	{
    	  Error_Handler();
    	}


}



void Send_CAN_Command(uint8_t function, uint16_t value){
	TxHeader.StdId=ID_MOTOR;
	TxHeader.DLC=4;
	TxData[0] = 0;
	TxData[1] = function;
	TxData[2] = (value>>8) & 0xff;
	TxData[3] = value & 0xff;

	// Start the Transmission process, zwei mal senden wie im Beispiel https://github.com/jliegner/bionxdrive
	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	  {
	   // Transmission request Error
	   Error_Handler();
	  }

}

int32_t map (int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  // if input is smaller/bigger than expected return the min/max out ranges value
  if (x < in_min)
    return out_min;
  else if (x > in_max)
    return out_max;

  // map the input to the output range.
  // round up if mapping bigger ranges to smaller ranges
  else  if ((in_max - in_min) > (out_max - out_min))
    return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
  // round down if mapping smaller ranges to bigger ranges
  else
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//PI control for smooth cut off of regen power with full battery
int16_t PI_control (int16_t ist, int16_t soll, int16_t max)
{

	float flt_p; //proportional part
	static float flt_i = 0; 			//integral part
	if(max == 0)flt_i=0; 				//reset integral part, if brake lever is released
	static float flt_pplusi = 0; 		// sum of proportional and integral part
	flt_p = (soll - ist)*P_FACTOR;
	flt_i += ((float)(soll - ist))*I_FACTOR;
	if(flt_i>63) flt_i=63;
	if(flt_i<-10) flt_i=-10;
	flt_pplusi = flt_p + flt_i;
	if(flt_pplusi >- max) flt_pplusi=-max;
	if(flt_pplusi < 0 ) flt_pplusi=0;

	return (-flt_pplusi);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
