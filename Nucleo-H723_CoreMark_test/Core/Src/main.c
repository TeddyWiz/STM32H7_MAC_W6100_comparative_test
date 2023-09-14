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
#include "cmsis_os.h"
#include "app_coremarkapp.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
ee_u32 CoreMark_cnt = 0;

//#include "stdio.h"
#include "w6100.h"
#include "wizchip_conf.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define SPI_DMA
#define Send_data_size 	 16000//7300//16000//10000
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

UART_HandleTypeDef huart3;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CoreMark */
osThreadId_t CoreMarkHandle;
const osThreadAttr_t CoreMark_attributes = {
  .name = "CoreMark",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
wiz_NetInfo gWIZNETINFO = { .mac = {0x00,0x08,0xdc,0xff,0xff,0xff},
							.ip = {192,168,10,111},
							.sn = {255, 255, 255, 0},
							.gw = {192, 168, 10, 1},
							.dns = {168, 126, 63, 1},
							.lla = {0xfe,0x80, 0x00,0x00,
								 0x00,0x00, 0x00,0x00,
								 0x02,0x08, 0xdc,0xff,
								 0xfe,0xff, 0xff,0xff},
				            .gua={0x20,0x01,0x02,0xb8,
								 0x00,0x10,0x00,0x01,
								 0x02,0x08,0xdc,0xff,
								 0xfe,0xff,0xff,0xff},
				            .sn6={0xff,0xff,0xff,0xff,
								 0xff,0xff,0xff,0xff,
								 0x00,0x00,0x00, 0x00,
								 0x00,0x00,0x00,0x00},
				            .gw6={0xfe, 0x80, 0x00,0x00,
								  0x00,0x00,0x00,0x00,
								  0x02,0x00, 0x87,0xff,
								  0xfe,0x08, 0x4c,0x81}
							};


uint8_t WIZ_Dest_IP[4] = {192, 168, 10, 2};                  //DST_IP Address
uint16_t WIZ_Dest_PORT = 5001;

uint8_t DestIP6_L[16] = {0xfe,0x80, 0x00,0x00,
						  0x00,0x00, 0x00,0x00,
                          0x31,0x71,0x98,0x05,
                          0x70,0x24,0x4b,0xb1
						};

uint8_t DestIP6_G[16] = {0x20,0x01,0x02,0xb8,
                          0x00,0x10,0x00,0x01,
                          0x31,0x71,0x98,0x05,
                          0x70,0x24,0x4b,0xb1
                         };

uint8_t Router_IP[16]= {0xff,0x02,0x00,0x00,
                          0x00,0x00,0x00,0x00,
                          0x00,0x00,0x00,0x00,
                          0x00,0x00,0x00,0x02
                         };
uint8_t data_buf[16384];
uint8_t SPI_DMA_flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_DMA_Init(void);
void StartDefaultTask(void *argument);
void StartCoreMark(void *argument);

/* USER CODE BEGIN PFP */

uint8_t rxData;
uint8_t rx_buffer[2048]= {0,};
int rx_index = 0;
uint8_t rx_flag =0;
uint16_t loop_mode = 0;
uint16_t temp_delay=0, temp_delay_dumi=0;
#ifdef KEIL
     #ifdef __GNUC__
     //With GCC, small printf (option LD Linker->Libraries->Small printf
     //set to 'Yes') calls __io_putchar()
         #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
		#else
				 #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
		#endif /* __GNUC__*/
#endif

#ifdef True_STD
	int _write(int fd, char *str, int len)
	{
		for(int i=0; i<len; i++)
		{
			HAL_UART_Transmit(&huart3, (uint8_t *)&str[i], 1, 0xFFFF);
		}
		return len;
	}
#endif

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

    /*
        This will be called once data is received successfully,
        via interrupts.
    */

     /*
       loop back received data
     */
     HAL_UART_Receive_IT(&huart3, &rxData, 1);
     if (rxData == '\n')
	 {
	   if (rx_buffer[rx_index - 1] == '\r')
	   {
		 rx_index--;
		 rx_flag = 1;
		 rx_buffer[rx_index] = 0;
	   }
	   else
	   {
		 rx_index = 0;
		 HAL_UART_Transmit(&huart3, "not support format\r\n", 20, 1000);
	   }
	 }
	 else if (rxData == 0x08) // back space
	 {
	   rx_index--;
	 }
	 else
	 {
	   rx_buffer[rx_index++] = rxData;
	 }
     HAL_UART_Transmit(&huart3, &rxData, 1, 1000);
}

void W6100CsEnable(void);
void W6100CsDisable(void);
#define W6100_CS_GPIO  1
#if 1
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	//printf("SPI DMA Complete\r\n");
	SPI_DMA_flag = 1;
}
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	SPI_DMA_flag = 1;
}
#endif
#if 0
uint8_t dma_write_data(uint32_t address, uint8_t *buff, uint16_t len)
{

	uint8_t *temp_pbuf = NULL;
	uint8_t *pbuf = (uint8_t*)calloc(len + 4, sizeof(uint8_t));
	temp_pbuf = pbuf;
	*temp_pbuf++ = (uint8_t)((address & 0x00FF0000) >> 16);
	*temp_pbuf++ = (uint8_t)((address & 0x0000FF00) >> 8);
	*temp_pbuf++ = (uint8_t)((address & 0x000000ff)|(_W6100_SPI_WRITE_ ));
	memcpy(temp_pbuf, buff, len);

	SPI_DMA_flag = 0;
	HAL_SPI_Transmit_DMA(&hspi1, pbuf, len+3);
	while(SPI_DMA_flag == 0);
	//while (HAL_DMA_GetState(hspi1.hdmatx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi1.hdmatx) == HAL_DMA_STATE_RESET);
	free(pbuf);

	return 0;
}
uint8_t dma_read_data(uint32_t address, uint8_t *buff, uint16_t len)
{
	//uint8_t temp_pbuf[3] = {0,};
	//uint8_t *pbuf = (uint8_t*)calloc(len + 4, sizeof(uint8_t));
	uint8_t *temp_pbuf = (uint8_t*)calloc(len + 4, sizeof(uint8_t));
	uint8_t *temp_pbuf1 = (uint8_t*)calloc(len + 4, sizeof(uint8_t));
	*temp_pbuf = (uint8_t)((address & 0x00FF0000) >> 16);
	*(temp_pbuf + 1) = (uint8_t)((address & 0x0000FF00) >> 8);
	*(temp_pbuf + 2) = (uint8_t)((address & 0x000000ff)|(_W6100_SPI_READ_ ));
	//memcpy(temp_pbuf, buff, len);
	//HAL_SPI_TransmitReceive_DMA_1(&hspi1,temp_pbuf, buff, 3, len);
	SPI_DMA_flag = 0;
	while (HAL_SPI_TransmitReceive_DMA(&hspi1,temp_pbuf, temp_pbuf1, len + 3) != HAL_OK);
	while(SPI_DMA_flag == 0);
	//while (HAL_DMA_GetState(hspi1.hdmarx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi1.hdmarx) == HAL_DMA_STATE_RESET);
	memcpy(buff,temp_pbuf1 + 3, len);
	free(temp_pbuf);
	free(temp_pbuf1);
	//free(pbuf);
	return len;
}
#else
#define spi_delay_en 0
uint8_t temp_pbuf[8024]={0,}; //16384+3
uint8_t temp_pbuf1[8024]={0,};
uint8_t dma_write_data(uint32_t address, uint8_t *buff, uint16_t len)
{
	temp_pbuf[0] = (uint8_t)((address & 0x00FF0000) >> 16);
	temp_pbuf[1] = (uint8_t)((address & 0x0000FF00) >> 8);
	temp_pbuf[2] = (uint8_t)((address & 0x000000ff)|(_W6100_SPI_WRITE_ ));
	memcpy(temp_pbuf+3, buff, len);
#if spi_delay_en
  for(temp_delay=0;temp_delay<10; temp_delay++)
  {
    temp_delay_dumi++;
  }
#endif
#if 0
	SPI_DMA_flag = 0;
	HAL_SPI_Transmit_DMA(&hspi1, temp_pbuf, len+3);
	while(SPI_DMA_flag == 0);
#else
  #if W6100_CS_GPIO
  W6100CsEnable();
  #endif
#if 1
	HAL_SPI_Transmit(&hspi1, temp_pbuf, len+3, 10);
#else
	SPI_DMA_flag = 0;
	HAL_SPI_Transmit_DMA(&hspi1, temp_pbuf, len+3);
	while(SPI_DMA_flag == 0);
#endif
  #if W6100_CS_GPIO
  W6100CsDisable();
  #endif
#endif
#if spi_delay_en
  for(temp_delay=0;temp_delay<10; temp_delay++)
  {
    temp_delay_dumi++;
  }
#endif
	//while (HAL_DMA_GetState(hspi1.hdmatx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi1.hdmatx) == HAL_DMA_STATE_RESET);
	return 0;
}
uint8_t dma_read_data(uint32_t address, uint8_t *buff, uint16_t len)
{
  temp_pbuf[0] = (uint8_t)((address & 0x00FF0000) >> 16);
	temp_pbuf[1] = (uint8_t)((address & 0x0000FF00) >> 8);
	temp_pbuf[2] = (uint8_t)((address & 0x000000ff)|(_W6100_SPI_READ_ ));
#if spi_delay_en
  for(temp_delay=0;temp_delay<10; temp_delay++)
  {
    temp_delay_dumi++;
  }
#endif
	SPI_DMA_flag = 0;
  #if 0
	HAL_SPI_TransmitReceive_DMA(&hspi1,temp_pbuf, temp_pbuf1, len + 3);
	while(SPI_DMA_flag == 0);
  #else
  #if W6100_CS_GPIO
  W6100CsEnable();
  #endif
#if 1
  HAL_SPI_TransmitReceive(&hspi1,temp_pbuf, temp_pbuf1, len + 3, 10);
#else
  SPI_DMA_flag = 0;
  HAL_SPI_TransmitReceive_DMA(&hspi1,temp_pbuf, temp_pbuf1, len + 3);
  	while(SPI_DMA_flag == 0);
#endif
  #if W6100_CS_GPIO
  W6100CsDisable();
  #endif
  #endif
	//while (HAL_DMA_GetState(hspi1.hdmarx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi1.hdmarx) == HAL_DMA_STATE_RESET);
	memcpy(buff,temp_pbuf1 + 3, len);
#if spi_delay_en
  for(temp_delay=0;temp_delay<10; temp_delay++)
  {
    temp_delay_dumi++;
  }
#endif
	return len;
}
#endif
void print_help_menu(void);
uint16_t CLI_Process(void);
uint8_t check_break(void)
{
  uint16_t cmd_mode;
  if (rx_flag == 1)
  {
    rx_flag = 0;
    cmd_mode=CLI_Process();
    if(cmd_mode ==4)
      return 1;
  }
  return 0;
}
void end_sig(void)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)"1\r\n", 3, 0xFFFF);
}
#if 1
void TRACE_ON(void)
{
	HAL_GPIO_WritePin(Trace_GPIO_Port, Trace_Pin, GPIO_PIN_RESET);
}
void TRACE_OFF(void)
{
	HAL_GPIO_WritePin(Trace_GPIO_Port, Trace_Pin, GPIO_PIN_SET);
}
#endif
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
	uint8_t temp_ver[2]={0,};
	PLL2_ClocksTypeDef PLL2_Clk_data;
	uint16_t temp_presc = 1, temp_presc_set, temp_presc_cnt = 0;
	int i = 0;
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  MX_DMA_Init();
  //MX_CoreMarkApp_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart3, &rxData, 1);
    printf("< W6100 clock TEST!! >\r\n");
    W6100CsDisable();
    HAL_RCCEx_GetPLL2ClockFreq(&PLL2_Clk_data);
      printf("SET PLL2 P:%ld, Q:%ld, R:%ld \r\n", PLL2_Clk_data.PLL2_P_Frequency, PLL2_Clk_data.PLL2_Q_Frequency, PLL2_Clk_data.PLL2_R_Frequency);
      temp_presc_set = (hspi1.Init.BaudRatePrescaler>>(4*7));
      printf("pre = %d\r\n", temp_presc_set);
      for(temp_presc_cnt = 0; temp_presc_cnt < temp_presc_set; temp_presc_cnt++)
      {
      	temp_presc = temp_presc*2;
      }
      printf("SPI CLK %d Mhz \r\n", (int)(PLL2_Clk_data.PLL2_P_Frequency / (2*temp_presc) / 1000000));
#if 0
    W6100CsEnable();
  	dma_read_data(0x000200, temp_ver, 2);
    W6100CsDisable();
  	printf("VER 0x%02X %02X\r\n", temp_ver[0], temp_ver[1]);
#endif
  	//int i = 0;
  		uint8_t syslock = SYS_NET_LOCK;
  		int8_t *data= NULL;
  		int32_t ret;
  		//uint8_t temp_ver[2]={0,};
  		printf("start W6100 \r\n");
  		W6100CsEnable();
  		dma_read_data(0x000200, temp_ver, 2);
  		W6100CsDisable();
  		printf("VER 0x%02X %02X\r\n", temp_ver[0], temp_ver[1]);
  		W6100Initialze();
  		ctlwizchip(CW_SYS_UNLOCK,& syslock);
  		ctlnetwork(CN_SET_NETINFO,&gWIZNETINFO);
  		for (i = 0; i < 8; i++)
  		  {
  			printf("%d : max size = %d k \r\n", i, getSn_TxMAX(i));
  		  }
  		printf("VERSION(%x) = %.2x \r\n", _VER_,getVER());
  		print_network_information();
  		printf("\r\n>");
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

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

  /* creation of CoreMark */
  CoreMarkHandle = osThreadNew(StartCoreMark, NULL, &CoreMark_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_CSI);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_CSI;
  RCC_OscInitStruct.CSIState = RCC_CSI_ON;
  RCC_OscInitStruct.CSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_CSI;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 275;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 0x0;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, USB_FS_PWR_EN_Pin|W6100_CS_Pin|Trace_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
  GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin RMII_CRS_DV_Pin */
  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin|RMII_MDIO_Pin|RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_GREEN_Pin LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin|LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RMII_TXD1_Pin */
  GPIO_InitStruct.Pin = RMII_TXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(RMII_TXD1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_FS_PWR_EN_Pin */
  GPIO_InitStruct.Pin = USB_FS_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_FS_PWR_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : W6100_CS_Pin Trace_Pin */
  GPIO_InitStruct.Pin = W6100_CS_Pin|Trace_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_FS_OVCR_Pin */
  GPIO_InitStruct.Pin = USB_FS_OVCR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_FS_OVCR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_FS_ID_Pin */
  GPIO_InitStruct.Pin = USB_FS_ID_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
  HAL_GPIO_Init(USB_FS_ID_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD0_Pin */
  GPIO_InitStruct.Pin = RMII_TX_EN_Pin|RMII_TXD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_YELLOW_Pin */
  GPIO_InitStruct.Pin = LED_YELLOW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_YELLOW_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

uint8_t W6100SpiReadByte(void)
{

	uint8_t rx = 0, tx = 0xFF;
	HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 1, 10);
	//HAL_SPI_Receive(&hspi1, &rx, 1, 10);
	return rx;

}

void W6100SpiWriteByte(uint8_t byte)
{
#ifdef USE_STDPERIPH_DRIVER

	while (SPI_I2S_GetFlagStatus(W6100_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(W6100_SPI, byte);
	while (SPI_I2S_GetFlagStatus(W6100_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(W6100_SPI);

#elif defined USE_HAL_DRIVER

	uint8_t rx;
	HAL_SPI_TransmitReceive(&hspi1, &byte, &rx, 1, 10);
#endif
    return 0;
}
void W6100SpiReadBurst(uint8_t* buf, datasize_t len)
{
	uint8_t tx[Send_data_size];
	HAL_SPI_TransmitReceive(&hspi1, tx, buf, len, 10);
	//HAL_SPI_Receive(&hspi1, buf, len, 10);
}
void W6100SpiWriteBurst(uint8_t* buf, datasize_t len)
{
	uint8_t rx[Send_data_size];
	HAL_SPI_TransmitReceive(&hspi1, buf, rx, len, 10);
}
void W6100CsEnable(void)
{
	HAL_GPIO_WritePin(W6100_CS_GPIO_Port, W6100_CS_Pin, GPIO_PIN_RESET);
}

void W6100CsDisable(void)
{
	HAL_GPIO_WritePin(W6100_CS_GPIO_Port, W6100_CS_Pin, GPIO_PIN_SET);
}

void W6100Initialze(void)
{
	//W6100Reset();

#if _WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_
/* SPI method callback registration */
	#if defined SPI_DMA
	reg_wizchip_spi_cbfunc(W6100SpiReadByte, W6100SpiWriteByte, W6100SpiReadBurst, W6100SpiWriteBurst);
	#else
	reg_wizchip_spi_cbfunc(W6100SpiReadByte, W6100SpiWriteByte, 0, 0);
	#endif
	/* CS function register */
	reg_wizchip_cs_cbfunc(W6100CsEnable, W6100CsDisable);
#else
/* Indirect bus method callback registration */
	#if defined BUS_DMA
	reg_wizchip_bus_cbfunc(W6100BusReadByte, W6100BusWriteByte, W6100BusReadBurst, W6100BusWriteBurst);
	#else
	reg_wizchip_bus_cbfunc(W6100BusReadByte, W6100BusWriteByte, 0, 0);
	#endif
#endif
	uint8_t temp;
	//unsigned char W6100_AdrSet[2][8] = {{2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}};
	unsigned char W6100_AdrSet[2][8] = {{16, 0, 0, 0, 0, 0, 0, 0}, {16, 0, 0, 0, 0, 0, 0, 0}};
	do
	{
		if (ctlwizchip(CW_GET_PHYLINK, (void *)&temp) == -1)
		{
			printf("Unknown PHY link status.\r\n");
		}
	} while (temp == PHY_LINK_OFF);
	printf("PHY OK.\r\n");

	temp = IK_DEST_UNREACH;

	if (ctlwizchip(CW_INIT_WIZCHIP, (void *)W6100_AdrSet) == -1)
	{
		printf("W6100 initialized fail.\r\n");
	}

	if (ctlwizchip(CW_SET_INTRMASK, &temp) == -1)
	{
		printf("W6100 interrupt\r\n");
	}
	//printf("interrupt mask: %02x\r\n",getIMR());
}
void print_network_information(void)
{
	wizchip_getnetinfo(&gWIZNETINFO);
	printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n\r",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("IP address : %d.%d.%d.%d\n\r",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("SN Mask	   : %d.%d.%d.%d\n\r",gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("Gate way   : %d.%d.%d.%d\n\r",gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("DNS Server : %d.%d.%d.%d\n\r",gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
	printf("LLA  : %.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X\r\n",gWIZNETINFO.lla[0],gWIZNETINFO.lla[1],gWIZNETINFO.lla[2],gWIZNETINFO.lla[3],\
									gWIZNETINFO.lla[4],gWIZNETINFO.lla[5],gWIZNETINFO.lla[6],gWIZNETINFO.lla[7],\
									gWIZNETINFO.lla[8],gWIZNETINFO.lla[9],gWIZNETINFO.lla[10],gWIZNETINFO.lla[11],\
									gWIZNETINFO.lla[12],gWIZNETINFO.lla[13],gWIZNETINFO.lla[14],gWIZNETINFO.lla[15]);
	printf("GUA  : %.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X\n\r",gWIZNETINFO.gua[0],gWIZNETINFO.gua[1],gWIZNETINFO.gua[2],gWIZNETINFO.gua[3],\
									gWIZNETINFO.gua[4],gWIZNETINFO.gua[5],gWIZNETINFO.gua[6],gWIZNETINFO.gua[7],\
									gWIZNETINFO.gua[8],gWIZNETINFO.gua[9],gWIZNETINFO.gua[10],gWIZNETINFO.gua[11],\
									gWIZNETINFO.gua[12],gWIZNETINFO.gua[13],gWIZNETINFO.gua[14],gWIZNETINFO.gua[15]);
	printf("SN6  : %.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X\n\r",gWIZNETINFO.sn6[0],gWIZNETINFO.sn6[1],gWIZNETINFO.sn6[2],gWIZNETINFO.sn6[3],\
									gWIZNETINFO.sn6[4],gWIZNETINFO.sn6[5],gWIZNETINFO.sn6[6],gWIZNETINFO.sn6[7],\
									gWIZNETINFO.sn6[8],gWIZNETINFO.sn6[9],gWIZNETINFO.sn6[10],gWIZNETINFO.sn6[11],\
									gWIZNETINFO.sn6[12],gWIZNETINFO.sn6[13],gWIZNETINFO.sn6[14],gWIZNETINFO.sn6[15]);
	printf("GW6  : %.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X:%.2X%.2X\r\n",gWIZNETINFO.gw6[0],gWIZNETINFO.gw6[1],gWIZNETINFO.gw6[2],gWIZNETINFO.gw6[3],\
									gWIZNETINFO.gw6[4],gWIZNETINFO.gw6[5],gWIZNETINFO.gw6[6],gWIZNETINFO.gw6[7],\
									gWIZNETINFO.gw6[8],gWIZNETINFO.gw6[9],gWIZNETINFO.gw6[10],gWIZNETINFO.gw6[11],\
									gWIZNETINFO.gw6[12],gWIZNETINFO.gw6[13],gWIZNETINFO.gw6[14],gWIZNETINFO.gw6[15]);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */

  /* Infinite loop */
  for(;;)
  {
	  tcps_status(0, 5001, AS_IPV4);
    //osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartCoreMark */
/**
* @brief Function implementing the CoreMark thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCoreMark */
void StartCoreMark(void *argument)
{
  /* USER CODE BEGIN StartCoreMark */
	printf("Start Core Mark Task\r\n");
  MX_CoreMarkApp_Init();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartCoreMark */
}

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

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
