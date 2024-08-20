/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

/* the usart2 receive datas buffer ,the max byte is define with USART2_REC_LEN  */
uint8_t g_usart2_rx_buf[USART2_REC_LEN];
/*  recive status
 *  bit15     received complete status
 *  bit14     received bit 0x0d
 *  bit13~0   the received significant bits
*/
uint8_t Uart2_Rx_Cnt=0;		//the uart2 receive count
uint16_t g_usart_rx_sta = 0;
uint8_t g_rx_buffer[RXBUFFERSIZE];  /* the global usart buffer receive  */
/*  redirect the printf point the GUN or the ac6 tool's implement
*/
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch,1,HAL_MAX_DELAY);
    return ch;
}


/* USER CODE END 0 */

UART_HandleTypeDef huart2;

/* USART2 init function */

void MX_USART2_UART_Init(void)
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

  /**/
  HAL_UART_Receive_IT(&huart2, (uint8_t *)g_usart2_rx_buf, USART2_REC_LEN);

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_USART2_ENABLE();

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */
//use usart2 when use usb device the usart will take the usb bus busy,
  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief       usart receive data callback function
                received data is handled in there
 * @param       huart: the usart handler
 * @retval      NULL
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */

	if(Uart2_Rx_Cnt >= 255)  //data overflow judge
	{
		Uart2_Rx_Cnt = 0;
		memset(g_rx_buffer,0x00,sizeof(g_rx_buffer));
		HAL_UART_Transmit(&huart2, (uint8_t *)"data overflow", 10,0xFFFF);

	}
	else
	{
		g_rx_buffer[Uart2_Rx_Cnt++] = g_usart2_rx_buf[0];   //save the data

		if((g_rx_buffer[Uart2_Rx_Cnt-1] == 0x0A)&&(g_rx_buffer[Uart2_Rx_Cnt-2] == 0x0D)) //judge end flag
		{
			HAL_UART_Transmit(&huart2, (uint8_t *)&g_rx_buffer, Uart2_Rx_Cnt,0xFFFF); //send the data back
            while(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);//check the usart2 send end
            Uart2_Rx_Cnt = 0;
			memset(g_rx_buffer,0x00,sizeof(g_rx_buffer)); //clear the buffer
		}
	}

	HAL_UART_Receive_IT(&huart2, (uint8_t *)&g_usart2_rx_buf, 1);   //restart the interrept
}

/*
 *printf in usat2 test A
*/
void USART_Hal_Test()
{
	printf("///\n\
	//                          _ooOoo_                          //\n\
	//                         o8888888o                         //\n\
	//                         88\" . \"88                         //\n\
	//                         (| ^_^ |)                         //\n\
	//                         O\\  =  /O                         //\n\
	//                      ____/`---'\\____                      //\n\
	//                    .'  \\\\|     |//  `.                    //\n\
	//                   /  \\\\|||  :  |||//  \\                   //\n\
	//                  /  _||||| -:- |||||-  \\                  //\n\
	//                  |   | \\\\\\  -  /// |   |                  //\n\
	//                  | \\_|  ''\\---/''  |   |                  //\n\
	//                  \\  .-\\__  `-`  ___/-. /                  //\n\
	//                ___`. .'  /--.--\\  `. . ___                //\n\
	//               ."" '<  `.___\\_<|>_/___.'  >'"".                //\n\
	//            | | :  `- \\`.;`\\ _ /`;.`/ - ` : | |            //\n\
	//            \\  \\ `-.   \\_ __\\ /__ _/   .-` /  /            //\n\
	//     ========`-.____`-.___\\_____/___.-`____.-'========     //\n\
	//                          `=---='                          //\n\
	//     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^     //\n\
	//           GODBLESS       NOCRASH     NOBUG             //\n\
	///\n");
}
/* USER CODE END 1 */
