#ifndef _LCD_H
#define _LCD_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

#define RS(data)  HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, (GPIO_PinState)data)  // 0:cmd 1:data
#define RW(data)  HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, (GPIO_PinState)data)  // 0:write 1:read
#define EN(data)  HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, (GPIO_PinState)data)  // enable

#define WORK_TIME_STR       "\xb9\xa4\xd7\xf7\xca\xb1\xbc\xe4\x3a\x20\x30\x30\x3a\x30\x30"
#define CUTTING_TIMES_STR   "\xc7\xd0\xb8\xee\xb5\xb6\xca\xfd\x3a\x20\x30\x30\x30\x30"
#define TOTAL_TIME_STR      "\xd7\xdc\xca\xb1\xb3\xa4\x3a\x20\x30\x30\x3a\x30\x30\x3a\x30\x30"

// "工作时间: 00:00".encode('gb2312')
// "切割刀数: 0000".encode('gb2312')
// "总时长: 00:00:00".encode('gb2312')

#define LINE1   0x80 //第一行起始地址，下同
#define LINE2   0x90
#define LINE3   0x88
#define LINE4   0x98

void lcd_port_init(void);
void lcd_check_busy(void);
void lcd_write_data(uint8_t data);
void lcd_write_cmd(uint8_t cmd);
void lcd_init(void);
void lcd_write_msg(uint8_t row, uint8_t column, const uint8_t *message);
void lcd_display_data(int32_t time, int32_t cutting_counter, int32_t total_time);

#endif

