/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dac.c
  * @brief   This file provides code for the configuration
  *          of the DAC instances.
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
#include "dac.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac_ch1;
DMA_HandleTypeDef hdma_dac_ch2;

/* DAC init function */
void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspInit 0 */

  /* USER CODE END DAC_MspInit 0 */
    /* DAC clock enable */
    __HAL_RCC_DAC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* DAC DMA Init */
    /* DAC_CH1 Init */
    hdma_dac_ch1.Instance = DMA2_Channel3;
    hdma_dac_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_dac_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dac_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dac_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_dac_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dac_ch1.Init.Mode = DMA_CIRCULAR;
    hdma_dac_ch1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_dac_ch1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(dacHandle,DMA_Handle1,hdma_dac_ch1);

    /* DAC_CH2 Init */
    hdma_dac_ch2.Instance = DMA2_Channel4;
    hdma_dac_ch2.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_dac_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dac_ch2.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dac_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_dac_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dac_ch2.Init.Mode = DMA_CIRCULAR;
    hdma_dac_ch2.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_dac_ch2) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(dacHandle,DMA_Handle2,hdma_dac_ch2);

  /* USER CODE BEGIN DAC_MspInit 1 */

  /* USER CODE END DAC_MspInit 1 */
  }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{

  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspDeInit 0 */

  /* USER CODE END DAC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC_CLK_DISABLE();

    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5);

    /* DAC DMA DeInit */
    HAL_DMA_DeInit(dacHandle->DMA_Handle1);
    HAL_DMA_DeInit(dacHandle->DMA_Handle2);
  /* USER CODE BEGIN DAC_MspDeInit 1 */

  /* USER CODE END DAC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */




/*DAC OUT1 DMA传输完成中断回调函数*/
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
    /*翻转RED_LED引脚状�??*/
	HAL_GPIO_TogglePin(led2_GPIO_Port, led2_Pin);
}


uint16_t Sine12bit_32[32] = { 2048, 2460, 2856, 3218, 3532, 3786, 3969, 4072, 4093,
		4031, 3887, 3668, 3382, 3042, 2661, 2255, 1841, 1435, 1054, 714, 428,
		209, 65, 3, 24, 127, 310, 564, 878, 1240, 1636, 2048 };

/*正弦波数据，12bit�????????????????1个周�????????????????128个点, 0-4095之间变化*/
const uint16_t Sine12bit_128[128] =
{
    2047,   2147,	2248,   2347,	2446,	2544,	2641,	2737,
    2830,   2922,	3012,	3099,	3184,	3266,	3346,	3422,
    3494,   3564,	3629,	3691,	3749,	3803,	3852,	3897,
    3938,   3974,	4006,	4033,	4055,	4072,	4084,	4092,
    4094,   4092,	4084,	4072,	4055,	4033,	4006,	3974,
    3938,   3897,	3852,	3803,	3749,	3691,	3629,	3564,
    3494,   3422,	3346,	3266,	3184,	3099,	3012,	2922,
    2830,   2737,	2641,	2544,	2446,	2347,	2248,	2147,
    2047,   1947,	1846,	1747,	1648,	1550,	1453,	1357,
    1264,   1172,	1082,	995 ,	910 ,	828 ,	748 ,	672 ,
    600 ,   530 ,	465 ,	403 ,	345 ,	291 ,	242 ,	197 ,
    156 ,   120 ,	88  ,	61  ,	39  ,	22  ,	10  ,	2   ,
    0   ,   2   ,	10  ,	22  ,	39  ,	61  ,	88  ,	120 ,
    156 ,   197 ,	242 ,	291 ,	345 ,	403 ,   465,	530 ,
    600 ,   672 ,	748 ,	828 ,	910 ,	995 ,	1082,	1172,
    1264,   1357,	1453,   1550,	1648,	1747,	1846,	1947
};

uint16_t Average12bit_32[32] = { 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048,
		2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048,
		2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048 };

uint16_t Averagefull12bit_32[32] = { 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,
		4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,
		4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000 };

uint16_t Averagemax12bit_32[32] = { 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094 };

uint16_t Averagefullzore12bit_32[32] = { 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };

uint16_t Ttriangular12bit_32[32] = {
		20, 270, 540, 810, 1080, 1350, 1620, 1570, 1940,
		2150, 2430, 2700, 2970, 3240, 3730, 4094, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20 };


/*
 * DAC GPIO test A generate a sin wave by the data
 * generate a static wave by the data
 */
void DAC_Hal_Test() {
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)Averagemax12bit_32, 32, DAC_ALIGN_12B_R);
	//HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)Sine12bit_32, 32, DAC_ALIGN_12B_R);
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)Average12bit_32, 32, DAC_ALIGN_12B_R);
}


/***************************************DAC out put the triangular wave*****************************************/







/* USER CODE END 1 */
