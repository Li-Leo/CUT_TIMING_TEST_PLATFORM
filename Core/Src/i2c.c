/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00100822;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
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
/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00100822;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#define I2C_MEMADD_SIZE		I2C_MEMADD_SIZE_8BIT                    // 小容量EEPROM芯片8位地�?用此参数
// #define I2C_MEMADD_SIZE I2C_MEMADD_SIZE_16BIT // 大容量EEPROM芯片16位地�?用此参数
#define ADDR_AT24C64_WRITE_ADDR 0xA0          // EEPROM I2C 写命令地�?
#define ADDR_AT24C64_READ_ADDR (0xA0 + 1)     // EEPROM I2C 读命令地�?
#define PAGE_SIZE 4

HAL_StatusTypeDef e2prom_write(uint16_t write_addr, void *p_data, uint32_t size)
{
    // HAL_StatusTypeDef ret = HAL_ERROR;
    uint16_t addr;
    uint8_t num_of_page, page_size = PAGE_SIZE, num_of_single, count = 0;
    uint8_t *p = (uint8_t *)p_data;

    addr = write_addr % page_size;
    count = page_size - addr;
    num_of_page = size / page_size;
    num_of_single = size % page_size;

    if (addr == 0) {
        if (num_of_page == 0) {
            HAL_I2C_Mem_Write(&hi2c1, ADDR_AT24C64_WRITE_ADDR, write_addr, I2C_MEMADD_SIZE, p, num_of_single, 100);
        } else {
            while (num_of_page--) {
                HAL_I2C_Mem_Write(&hi2c1, ADDR_AT24C64_WRITE_ADDR, write_addr, I2C_MEMADD_SIZE, p, page_size, 100);
                write_addr += page_size;
                p += page_size;
                HAL_Delay(5);
            }
            if (num_of_single) {
                HAL_I2C_Mem_Write(&hi2c1, ADDR_AT24C64_WRITE_ADDR, write_addr, I2C_MEMADD_SIZE, p, num_of_single, 100);
            }
        }
    } else {
        if (size > count) {
            size -= count;
            num_of_page = size / page_size;
            num_of_single = size % page_size;

            if (count) {
                HAL_I2C_Mem_Write(&hi2c1, ADDR_AT24C64_WRITE_ADDR, write_addr, I2C_MEMADD_SIZE, p, count, 100);
                write_addr += count;
                p += count;
                HAL_Delay(5);
            }

            while (num_of_page--) {
                HAL_I2C_Mem_Write(&hi2c1, ADDR_AT24C64_WRITE_ADDR, write_addr, I2C_MEMADD_SIZE, p, page_size, 100);
                write_addr += page_size;
                p += page_size;
                HAL_Delay(5);
            }

            if (num_of_single) {
                HAL_I2C_Mem_Write(&hi2c1, ADDR_AT24C64_WRITE_ADDR, write_addr, I2C_MEMADD_SIZE, p, num_of_single, 100);
            }
        } else {
            HAL_I2C_Mem_Write(&hi2c1, ADDR_AT24C64_WRITE_ADDR, write_addr, I2C_MEMADD_SIZE, p, size, 100);
        }
    }

    return HAL_OK;
}

HAL_StatusTypeDef e2prom_read(int addr, void *pData, int size)
{
    HAL_StatusTypeDef ret = HAL_ERROR;

    if (pData != NULL) {
        ret = HAL_I2C_Mem_Read(&hi2c1, ADDR_AT24C64_READ_ADDR, addr, I2C_MEMADD_SIZE, (uint8_t *)pData, size, 100);
        if (ret != HAL_OK) {
            printf("I2C_Read Sys Para read error\n");
            return HAL_ERROR;
        }
    } else {
        printf("I2C_Read pData NULL\n");
        return HAL_ERROR;
    }

    return HAL_OK;
}

void read_test(int addr, int size)
{
    uint8_t tmp[256];
    uint16_t num, mod;
    num = size / 256;
    mod = size % 256;

    while (num--) {
        e2prom_read(addr, tmp, 256);
        for (int i = 0; i < 256; i++) {
            if (i % 16 == 0)
            {
                printf("\n");
            }
            printf("%02x ", tmp[i]);
        }
        addr += 256;
    }

    if (mod) {
        e2prom_read(addr, tmp, mod);
        for (int i = 0; i < mod; i++) {
            if (i % 16 == 0)
            {
                printf("\n");
            }
            printf("%02x ", tmp[i]);
        }
    }
    printf("\n");
}

void write_test(int addr, int size)
{
    unsigned char sys_para[102] = {
        // 0x00, 0x00,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
        0x60, 0x61, 0x62, 0x63};

    e2prom_write(addr, sys_para, size);
    printf("\n");
}

/* USER CODE END 1 */
