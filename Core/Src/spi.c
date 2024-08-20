/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */

//W25Q128
#define SPI_CS_Enable() 		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)
#define SPI_CS_Disable() 		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ManufactDeviceID_CMD	0x90
#define READ_STATU_REGISTER_1   0x05
#define READ_STATU_REGISTER_2   0x35
#define READ_DATA_CMD	        0x03
#define WRITE_ENABLE_CMD	    0x06
#define WRITE_DISABLE_CMD	    0x04
#define SECTOR_ERASE_CMD	    0x20
#define CHIP_ERASE_CMD	        0xc7
#define PAGE_PROGRAM_CMD        0x02
/* USER CODE END PD */

/* Private user code ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint16_t device_id;
static uint8_t read_buf[10] = {0};
static uint8_t write_buf[10] = {0};
static int i;
/* USER CODE END PV */


/**
 * @brief    SPI send a define length data
 * @param    send_buf  - a send data buffer pointer
 * @param    size �? the sending data size
 * @retval   return 	HAL_OK(function is ok)
 */
static HAL_StatusTypeDef SPI_Transmit(uint8_t* send_buf, uint16_t size)
{
    return HAL_SPI_Transmit(&hspi2, send_buf, size, 100);
}

/**
 * @brief   SPI receive a define length data
 * @param   recv_buf  �?- a receive data buffer pointer
 * @param   size �? the receive data size
 * @retval  return 	HAL_OK(function is ok)
 */
static HAL_StatusTypeDef SPI_Receive(uint8_t* recv_buf, uint16_t size)
{
   return HAL_SPI_Receive(&hspi2, recv_buf, size, 100);
}

/**
 * @brief   SPI在发送数据的同时接收指定长度的数�??
 * @param   send_buf  —�?? 接收数据缓冲区首地址
 * @param   recv_buf  —�?? 接收数据缓冲区首地址
 * @param   size —�?? 要发�??/接收数据的字节数
 * @retval  成功返回HAL_OK
 */
static HAL_StatusTypeDef SPI_TransmitReceive(uint8_t* send_buf, uint8_t* recv_buf, uint16_t size)
{
   return HAL_SPI_TransmitReceive(&hspi2, send_buf, recv_buf, size, 100);
}

/**
 * @brief   读取Flash内部的ID
 * @param   none
 * @retval  成功返回device_id
 */
uint16_t W25QXX_ReadID(void)
{
    uint8_t recv_buf[2] = {0};    //recv_buf[0]存放Manufacture ID, recv_buf[1]存放Device ID
    uint16_t device_id = 0;
    uint8_t send_data[4] = {ManufactDeviceID_CMD,0x00,0x00,0x00};   //待发送数据，命令+地址

    /* 使能片�?? */
    SPI_CS_Enable();

    /* 发�?�并读取数据 */
    if (HAL_OK == SPI_Transmit(send_data, 4))
    {
        if (HAL_OK == SPI_Receive(recv_buf, 2))
        {
            device_id = (recv_buf[0] << 8) | recv_buf[1];
        }
    }

    /* 取消片�?? */
    SPI_CS_Disable();

    return device_id;
}

/**
 * @brief     读取W25QXX的状态寄存器，W25Q64�??共有2个状态寄存器
 * @param     reg  —�?? 状�?�寄存器编号(1~2)
 * @retval    状�?�寄存器的�??
 */
static uint8_t W25QXX_ReadSR(uint8_t reg)
{
    uint8_t result = 0;
    uint8_t send_buf[4] = {0x00,0x00,0x00,0x00};
    switch(reg)
    {
        case 1:
            send_buf[0] = READ_STATU_REGISTER_1;
        case 2:
            send_buf[0] = READ_STATU_REGISTER_2;
        case 0:
        default:
            send_buf[0] = READ_STATU_REGISTER_1;
    }

     /* 使能片�?? */
    SPI_CS_Enable();

    if (HAL_OK == SPI_Transmit(send_buf, 4))
    {
        if (HAL_OK == SPI_Receive(&result, 1))
        {
            SPI_CS_Disable();

            return result;
        }
    }

    /* 取消片�?? */
    SPI_CS_Disable();

    return 0;
}

/**
 * @brief	阻塞等待Flash处于空闲状�??
 * @param   none
 * @retval  none
 */
static void W25QXX_Wait_Busy(void)
{
    while((W25QXX_ReadSR(1) & 0x01) == 0x01); // 等待BUSY位清�??
}


/**
 * @brief   读取SPI FLASH数据
 * @param   buffer      —�?? 数据存储�??
 * @param   start_addr  —�?? �??始读取的地址(�??�??32bit)
 * @param   nbytes      —�?? 要读取的字节�??(�??�??65535)
 * @retval  成功返回0，失败返�??-1
 */
int W25QXX_Read(uint8_t* buffer, uint32_t start_addr, uint16_t nbytes)
{
    uint8_t cmd = READ_DATA_CMD;

    start_addr = start_addr << 8;

	W25QXX_Wait_Busy();

     /* 使能片�?? */
    SPI_CS_Enable();

    SPI_Transmit(&cmd, 1);

    if (HAL_OK == SPI_Transmit((uint8_t*)&start_addr, 3))
    {
        if (HAL_OK == SPI_Receive(buffer, nbytes))
        {
            SPI_CS_Disable();
            return 0;
        }
    }

    SPI_CS_Disable();
    return -1;
}

/**
 * @brief    W25QXX写使�??,将S1寄存器的WEL置位
 * @param    none
 * @retval
 */
void W25QXX_Write_Enable(void)
{
    uint8_t cmd= WRITE_ENABLE_CMD;

    SPI_CS_Enable();

    SPI_Transmit(&cmd, 1);

    SPI_CS_Disable();

    W25QXX_Wait_Busy();

}

/**
 * @brief    W25QXX写禁�??,将WEL清零
 * @param    none
 * @retval    none
 */
void W25QXX_Write_Disable(void)
{
    uint8_t cmd = WRITE_DISABLE_CMD;

    SPI_CS_Enable();

    SPI_Transmit(&cmd, 1);

    SPI_CS_Disable();

    W25QXX_Wait_Busy();
}
/**
 * @brief    W25QXX擦除�??个扇�??
 * @param   sector_addr    —�?? 扇区地址 根据实际容量设置
 * @retval  none
 * @note    阻塞操作
 */
void W25QXX_Erase_Sector(uint32_t sector_addr)
{
    uint8_t cmd = SECTOR_ERASE_CMD;

    sector_addr *= 4096;    //每个块有16个扇区，每个扇区的大小是4KB，需要换算为实际地址
    sector_addr <<= 8;

    W25QXX_Write_Enable();  //擦除操作即写�??0xFF，需要开启写使能
    W25QXX_Wait_Busy();        //等待写使能完�??

     /* 使能片�?? */
    SPI_CS_Enable();

    SPI_Transmit(&cmd, 1);

    SPI_Transmit((uint8_t*)&sector_addr, 3);

    SPI_CS_Disable();

    W25QXX_Wait_Busy();       //等待扇区擦除完成
}
/**
 * @brief    页写入操�??
 * @param    dat —�?? 要写入的数据缓冲区首地址
 * @param    WriteAddr —�?? 要写入的地址
 * @param   byte_to_write —�?? 要写入的字节数（0-256�??
 * @retval    none
 */
void W25QXX_Page_Program(uint8_t* dat, uint32_t WriteAddr, uint16_t nbytes)
{
    uint8_t cmd = PAGE_PROGRAM_CMD;

    WriteAddr <<= 8;

    W25QXX_Write_Enable();

    /* 使能片�?? */
    SPI_CS_Enable();

    SPI_Transmit(&cmd, 1);

    SPI_Transmit((uint8_t*)&WriteAddr, 3);

    SPI_Transmit(dat, nbytes);

    SPI_CS_Disable();

    W25QXX_Wait_Busy();
}


/* USER CODE END 0 */

SPI_HandleTypeDef hspi2;

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


/**
  * @brief  The application of SPI, read and write  spiflash W25Q64.
  * @retval void
  */
void SPI_Hal_Test(){
	device_id = W25QXX_ReadID();
	  printf("W25Q64 Device ID is 0x%04x\r\n", device_id);

	  /* 为了验证，首先读取要写入地址处的数据
	   * first of all ,you should read the data from the addr
	   * */
	  printf("-------- read data before write -----------\r\n");
	  W25QXX_Read(read_buf, 0, 10);

	  for(i = 0; i < 10; i++)
	  {
	    printf("[0x%08x]:0x%02x\r\n", i, *(read_buf+i));
	  }

	  /* 擦除该扇�??
	   * erase the sector
	   *
	   * */
	  printf("-------- erase sector 0 -----------\r\n");
	  W25QXX_Erase_Sector(0);

	  /* 再次读数�?? */
	  printf("-------- read data after erase -----------\r\n");
	  W25QXX_Read(read_buf, 0, 10);
	  for(i = 0; i < 10; i++)
	  {
	    printf("[0x%08x]:0x%02x\r\n", i, *(read_buf+i));
	  }

	  /* 写数�?? */
	  printf("-------- write data -----------\r\n");
	  for(i = 0; i < 10; i++)
	  {
	    write_buf[i] = i;
	  }
	  W25QXX_Page_Program(write_buf, 0, 10);

	  /* 再次读数�?? */
	  printf("-------- read data after write -----------\r\n");
	  W25QXX_Read(read_buf, 0, 10);
	  for(i = 0; i < 10; i++)
	  {
	    printf("[0x%08x]:0x%02x\r\n", i, *(read_buf+i));
	  }

}



/* USER CODE END 1 */
