/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>


/* USER CODE END Includes */

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
#define USART2_REC_LEN              1           /* define the usart2 max receive byte number 1 */
#define USART2_EN_RX                1           /* enable () or disable () the USART2 receive data*/
#define RXBUFFERSIZE                256         /* usart receive buffer max size */

extern uint8_t Uart2_Rx_Cnt;		//usart2 receive data count
extern uint8_t  g_usart2_rx_buf[USART2_REC_LEN];  /* the usart2 receive datas buffer ,the max byte is define with USART2_REC_LEN ,return with '\n; */
extern uint16_t g_usart2_rx_sta;                  /*the usart2 receive date status  */
extern uint8_t g_rx_buffer[RXBUFFERSIZE];         /* the global usart buffer receive  */



/* USER CODE END Private defines */

void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
 extern void USART_Hal_Test();
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

