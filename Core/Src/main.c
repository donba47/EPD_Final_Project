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
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "stm32f429i_discovery_lcd.h"
#include <stdio.h>
#include "./image/display_1.h"
#include "./image/display_2.h"

/* ESP8266 part(start) */
void ESP8266_Init();    // ??��?? ESP8266 (?��?��RST Pin)
void ESP8266_SendData(int16_t sensor1_data,int16_t sensor2_data);    // ?��??4條AT??�令
int32_t checkBufferCharacters(uint8_t* buffer, const char* targetString);
// ?��來�?��?��?��?�陣??�中??�內�??????
// ，是?��跟targetString�??????�??????
// ，是??�傳1，否??�傳0
/* ESP8266 part(end) */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define DHT22_PORT GPIOC
#define DHT22_PIN GPIO_PIN_3

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t 	stimebuf[30]={0};
uint8_t 	sdatebuf[30]={0};
RTC_TimeTypeDef sTime={0};
RTC_DateTypeDef sDate={0};

/* ESP8266 part(start) */
uint8_t at_inst1[]="AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n";
char at_inst2[16];
uint8_t at_inst4[]="AT+CIPCLOSE\r\n";

#define BUFFER_SIZE 64
uint8_t rx_buffer_init[128];   //reset後AT??�令??�傳??��?�息buffer
uint8_t rx_buffer1[BUFFER_SIZE];    //1st AT??�令??�傳??��?�息buffer
uint8_t rx_buffer2[BUFFER_SIZE];    //2nd AT??�令??�傳??��?�息buffer
uint8_t rx_buffer3[BUFFER_SIZE];    //3th AT??�令??�傳??��?�息buffer
uint8_t rx_buffer4[BUFFER_SIZE];    //4th AT??�令??�傳??��?�息buffer

//?��ESP8266死�?�跳?��?��，當ESP8266�??????確�?��?��?�set 1，否???0 -> 跳脫
int32_t init_receive_ok=0;
int32_t inst1_receive_ok=0;
int32_t inst2_receive_ok=0;
int32_t inst3_receive_ok=0;
int32_t inst4_receive_ok=0;

int32_t timeout=0;    //記數?��(每�?��?�令?��?�失??��?��?�都??��?�傳五次)

/* ESP8266 part(end) */

/* DHT11 part(start) */
void delay (uint16_t time);
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void DHT22_Start (void);
uint8_t DHT22_Check_Response (void);
uint8_t DHT22_Read (void);
uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM, RH, TEMP;
int16_t Temperature = 0;
int16_t Humidity = 0;
uint8_t Presence = 0;
int32_t timeout_DHT=0;
/* DHT11 part(end) */

uint8_t humidityString[10] = {0};
uint8_t TemperatureString[10] = {0};

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
  MX_DMA2D_Init();
  MX_LTDC_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_SPI5_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  BSP_LCD_Init();

  BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_GRAY);
  HAL_TIM_Base_Start(&htim6);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	  sprintf ((char*)stimebuf,"%02u:%02u",sTime.Hours, sTime.Minutes);
	  BSP_LCD_DrawRect(0,0,240,18);
	  BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	  BSP_LCD_FillRect(0,0,240,18);
	  BSP_LCD_SetFont(&Font16);
	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	  BSP_LCD_SetBackColor(LCD_COLOR_DARKGRAY);
	  BSP_LCD_DisplayStringAt(0, 0, stimebuf, RIGHT_MODE);

	  sprintf ((char*)sdatebuf,"%02u/%02u/%02u",sDate.Year, sDate.Month,sDate.Date);
	  BSP_LCD_SetFont(&Font16);
	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	  BSP_LCD_SetBackColor(LCD_COLOR_DARKGRAY);
	  BSP_LCD_DisplayStringAt(0, 0, sdatebuf, LEFT_MODE);
	  //
	  //
	  BSP_LCD_SetFont(&Font24);
	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	  BSP_LCD_SetBackColor(LCD_COLOR_DARKGRAY);
	  BSP_LCD_DisplayStringAtLine(2, (uint8_t*) "Temperature       ");
	  //
	  BSP_LCD_SetFont(&Font24);
	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	  BSP_LCD_SetBackColor(LCD_COLOR_DARKGRAY);
	  BSP_LCD_DisplayStringAtLine(5, (uint8_t*) "Humidity         ");
	  //
	  BSP_LCD_SetFont(&Font24);
	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	  BSP_LCD_SetBackColor(LCD_COLOR_DARKGRAY);
	  BSP_LCD_DisplayStringAtLine(8, (uint8_t*) "State             ");
	  //
	  BSP_LCD_DrawRect(95,233,20,20);
	  BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
	  BSP_LCD_FillRect(95,233,20,20);
	  BSP_LCD_SetFont(&Font20);
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_SetBackColor(LCD_COLOR_GRAY);
	  BSP_LCD_DisplayStringAt(0, 235, (uint8_t*)"Sensor", LEFT_MODE);
	  //
	  BSP_LCD_SetFont(&Font16);
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_SetBackColor(LCD_COLOR_GRAY);
	  BSP_LCD_DisplayStringAt(0, 270, (uint8_t*)"    :Done", LEFT_MODE);
	  BSP_LCD_DrawRect(27,267,15,15);
	  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	  BSP_LCD_FillRect(27,267,15,15);
	  //
	  BSP_LCD_SetFont(&Font16);
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_SetBackColor(LCD_COLOR_GRAY);
	  BSP_LCD_DisplayStringAt(0, 270, (uint8_t*)":Working ", RIGHT_MODE);
	  BSP_LCD_DrawRect(115,267,15,15);
	  BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
	  BSP_LCD_FillRect(115,267,15,15);
	  //
	  BSP_LCD_SetFont(&Font16);
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_SetBackColor(LCD_COLOR_GRAY);
	  BSP_LCD_DisplayStringAt(0, 295, (uint8_t*)"   :Failed", CENTER_MODE);
	  BSP_LCD_DrawRect(75,292,15,15);
	  BSP_LCD_SetTextColor(LCD_COLOR_RED);
	  BSP_LCD_FillRect(75,292,15,15);
	  //
	  HAL_Delay(1500);
	  DHT22_Start();
	  Presence = DHT22_Check_Response();
	  if (Presence == 0)
	  {
		  HAL_Delay(3000);
		  timeout_DHT=0;
		  continue;
	  }
	  Rh_byte1 = DHT22_Read ();
	  Rh_byte2 = DHT22_Read ();
	  Temp_byte1 = DHT22_Read ();
	  Temp_byte2 = DHT22_Read ();
	  SUM = DHT22_Read();
	  if (SUM == (Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2))
	  {
	  Temperature = (Temp_byte1<<8|Temp_byte2)/10;
	  Humidity = (Rh_byte1<<8|Rh_byte2)/10;
	  //
	  BSP_LCD_DrawRect(95,233,20,20);
	  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	  BSP_LCD_FillRect(95,233,20,20);
	  }
	  //Temperature++;
	  //Humidity++;
	  //
	  sprintf((char*)TemperatureString, "%d.c", Temperature);
	  BSP_LCD_SetFont(&Font24);
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_SetBackColor(LCD_COLOR_GRAY);
	  BSP_LCD_DisplayStringAt(0, 90, TemperatureString, CENTER_MODE);
	  //
	  sprintf((char*)humidityString, "%d%%", Humidity);
	  BSP_LCD_SetFont(&Font24);
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_SetBackColor(LCD_COLOR_GRAY);
	  BSP_LCD_DisplayStringAt(0,162,humidityString, CENTER_MODE);
	  HAL_Delay(1000);
	  //
	  BSP_LCD_SetFont(&Font20);
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_SetBackColor(LCD_COLOR_GRAY);
	  BSP_LCD_DisplayStringAt(0, 235, (uint8_t*)"Upload  ", RIGHT_MODE);
	  BSP_LCD_DrawRect(215,233,20,20);
	  BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
	  BSP_LCD_FillRect(215,233,20,20);
	  //
	  ESP8266_Init();
	  ESP8266_SendData(Temperature, Humidity);
	  Presence = 0;
	  HAL_Delay(1500);
	  init_receive_ok=0;

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

/* ESP8266 part(start) */
void ESP8266_Init(){
    memset(rx_buffer_init, 0, sizeof(rx_buffer_init));
    init_receive_ok=0;

    while(init_receive_ok!=1){

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);    //讓RST Pin = Low
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);    //讓RST Pin = High
	HAL_UART_Receive(&huart1, rx_buffer_init, sizeof(rx_buffer_init)-1, 1000);    //Reset 後接?��ESP01??�傳??��?�息
	init_receive_ok=checkBufferCharacters(rx_buffer_init, "SystemReady");//?�� Reset 後接?��??��?�息?��"SystemReady"，init_receive_ok=1，跳?��迴�??
	}
}

void ESP8266_SendData(int16_t sensor1_data,int16_t sensor2_data) {
    //??��?��?��?�傳ok??��?�數
    inst1_receive_ok=0;
    inst2_receive_ok=0;
    inst3_receive_ok=0;
    inst4_receive_ok=0;
    HAL_Delay(5000);

    /*?���?????? 1st AT ??�令*/
    timeout=0;    //??�傳計數�??????0
    while(inst1_receive_ok==0 && timeout<=5){    //?��??5次�?�直?��ESP01??��?�正�??????
	HAL_UART_Transmit(&huart1, at_inst1, sizeof(at_inst1) - 1, 1000);
	HAL_UART_Receive(&huart1, rx_buffer1, sizeof(rx_buffer1)-1, 1000);
	inst1_receive_ok = checkBufferCharacters(rx_buffer1, "Linked")||checkBufferCharacters(rx_buffer1, "CONNECT");
	HAL_Delay(3000);
	timeout++;
    }
    if(inst1_receive_ok!=1){    //如�?�傳??5次都失�?��?�直?��跳出ESP8266_SendData?���??????
    	BSP_LCD_DrawRect(215,233,20,20);
    	BSP_LCD_SetTextColor(LCD_COLOR_RED);
    	BSP_LCD_FillRect(215,233,20,20);
    	return;
    }
    /*?���?????? 1st AT ??�令*/

    /*組�?? 3rd AT ??�令*/
    uint8_t sensor1_name[]="&field1=";
    uint8_t sensor2_name[]="&field2=";
    uint8_t at_inst3_temp1[]="GET /update?api_key=V8CUO3DJT76JJ3XW";
    int16_t at_inst3_length=0;
    int16_t inst3_length=0;

    at_inst3_length=0;
    inst3_length=0;
    at_inst3_length = strlen((const char*)at_inst3_temp1) + strlen((const char*)sensor1_name) + strlen((const char*)&sensor1_data)+ strlen((const char*)sensor2_name) + strlen((const char*)&sensor2_data)+4;    //計�?�inst3??��?�串總長度�??+4?��\r\n
    uint8_t at_inst3[at_inst3_length];    //�????????��??��?�inst3字串?��度�?�好??��?��?�陣???

    sprintf((char*)at_inst3,"GET /update?api_key=V8CUO3DJT76JJ3XW%s%d%s%d\r\n",(char*)sensor1_name,sensor1_data,(char*)sensor2_name,sensor2_data);    //組�?�inst3
    /*組�?? 3rd AT ??�令*/

    /*組�?? 2rd AT ??�令*/
    inst3_length = strlen((char*)at_inst3);    //計�?��?��?��?�inst3??�總?���??????
    sprintf(at_inst2, "AT+CIPSEND=%d\r\n",inst3_length);  //組�?�inst2
    /*組�?? 2rd AT ??�令*/

    /*?��?? 2rd AT ??�令*/
    timeout=0;
    while(inst2_receive_ok==0 && timeout<=5){
    HAL_UART_Transmit(&huart1, (uint8_t*)at_inst2, sizeof(at_inst2) - 1, 1000);
    HAL_UART_Receive(&huart1, rx_buffer2, sizeof(rx_buffer2)-1, 1000);
    inst2_receive_ok = checkBufferCharacters(rx_buffer2, ">");
    HAL_Delay(3000);
    timeout++;
    }
    if(inst2_receive_ok!=1){
    	BSP_LCD_DrawRect(215,233,20,20);
    	BSP_LCD_SetTextColor(LCD_COLOR_RED);
    	BSP_LCD_FillRect(215,233,20,20);
    	return;
    }
    HAL_Delay(1000);
    /*?��?? 2rd AT ??�令*/

    /*?��?? 3th AT ??�令*/
    timeout=0;
    while(inst3_receive_ok==0 && timeout<=5)
    {
        HAL_UART_Transmit(&huart1, (uint8_t*)at_inst3, inst3_length, 1000);
        HAL_UART_Receive(&huart1, rx_buffer3, sizeof(rx_buffer3)-1, 1000);
        inst3_receive_ok = checkBufferCharacters(rx_buffer3, "SENDOKIPD");
        HAL_Delay(3000);
        timeout++;
    }
    if(inst3_receive_ok==1)
    {
    	BSP_LCD_DrawRect(215,233,20,20);
    	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    	BSP_LCD_FillRect(215,233,20,20);
    }
    else if(inst3_receive_ok!=1)
    {
    	BSP_LCD_DrawRect(215,233,20,20);
    	BSP_LCD_SetTextColor(LCD_COLOR_RED);
    	BSP_LCD_FillRect(215,233,20,20);
    	return;
    }

    HAL_Delay(1000);
    /*?��?? 3th AT ??�令*/

    /*?��?? 4th AT ??�令*/
    timeout=0;
    while(inst4_receive_ok==0 && timeout<=5){
	HAL_UART_Transmit(&huart1, (uint8_t*)at_inst4, sizeof(at_inst4) - 1, 1000);
	HAL_UART_Receive(&huart1, rx_buffer4, sizeof(rx_buffer4)-1, 1000);
	inst4_receive_ok = checkBufferCharacters(rx_buffer4, "A");
	HAL_Delay(3000);
	timeout++;
    }
    if(inst4_receive_ok!=1){
	return;
	}
    HAL_Delay(1000);
    /*?��?? 4th AT ??�令*/

    /*清空?��?��??�令??�buffer*/
    memset(rx_buffer1, 0, sizeof(rx_buffer1));
    memset(rx_buffer2, 0, sizeof(rx_buffer2));
    memset(rx_buffer3, 0, sizeof(rx_buffer3));
    memset(rx_buffer4, 0, sizeof(rx_buffer4));
}

int32_t checkBufferCharacters(uint8_t* buffer, const char* targetString){
    uint32_t targetLen = strlen(targetString);

    for (uint32_t i = 0; i < targetLen; i++){
        int found = 0;
        for (uint32_t j = 0; j < BUFFER_SIZE; j++){
            if (buffer[j] == targetString[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return 0;
        }
    }
    return 1;
}
/* ESP8266 part(end) */

/* DHT22 part(start) */
void delay (uint16_t time)
{
	/* change your code here for the delay in microseconds */
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim6))<time);
}

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	//GPIO_NOPULL
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DHT22_Start (void)
{
	Set_Pin_Output (DHT22_PORT, DHT22_PIN);  // set the pin as output
	HAL_GPIO_WritePin (DHT22_PORT, DHT22_PIN, 0);   // pull the pin low
	delay (1200);   // wait for 18ms
    HAL_GPIO_WritePin (DHT22_PORT, DHT22_PIN, 1);   // pull the pin high
	delay (20);   // wait for 20us
	Set_Pin_Input(DHT22_PORT, DHT22_PIN);    // set as input
}

uint8_t DHT22_Check_Response (void)
{
    uint8_t Response = 0;
    timeout_DHT=0;
    while(Response==0 && timeout_DHT<5){
	if ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN))==0)
	{
            delay (80);

	    if ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN))==1)
	    	 Response = 1;

	    else Response = -1; // 255
	}
	else
	{
		delay (5);
		timeout_DHT++;
	}
    }
    return Response;
}

 uint8_t DHT22_Read (void)
{
    uint8_t i,j;
    while ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)));   // wait for the pin to go low
    for (j=0;j<8;j++)
    {
	    while (!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)));   // wait for the pin to go high
	    delay (40);   // wait for 40 us
	    if (!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)))   // if the pin is low
	{
		i&= ~(1<<(7-j));   // write 0
	}
	    else i|= (1<<(7-j));  // if the pin is high, write 1
    while ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)));  // wait for the pin to go low
    }
    return i;
}
/* DHT22 part(end) */

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

