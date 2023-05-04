#ifndef _LCD_H
#define _LCD_H

#include <stdint.h>
#include "main.h"

#define RS(data)  HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, (GPIO_PinState)data)  // 0:cmd 1:data
#define RW(data)  HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, (GPIO_PinState)data)  // 0:write 1:read
#define EN(data)  HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, (GPIO_PinState)data)  // enable

#define LINE1   0x80 //第一行起始地址，下同
#define LINE2   0x90
#define LINE3   0x88
#define LINE4   0x98

void lcd_port_init(void);
void lcd_check_busy(void);
void lcd_write_data(uint8_t data);
void lcd_write_cmd(uint8_t cmd);
void lcd_init(void);
void lcd_write_msg(uint8_t *message, uint8_t address);

#endif

