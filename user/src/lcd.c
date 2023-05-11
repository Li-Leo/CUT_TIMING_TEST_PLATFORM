#include "lcd.h"

void ssz_delay_us(int ms);

void lcd_data_port_init_ouput(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pin = DB0_Pin|DB1_Pin|DB2_Pin|DB3_Pin|DB4_Pin|DB5_Pin|DB6_Pin|DB7_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void lcd_data_port_init_input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pin = DB0_Pin|DB1_Pin|DB2_Pin|DB3_Pin|DB4_Pin|DB5_Pin|DB6_Pin|DB7_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void lcd_init()
{
    lcd_data_port_init_ouput();

    ssz_delay_us(50000);

    lcd_write_cmd(0x30); // 功能设定：基本指令集
    ssz_delay_us(200);
    lcd_write_cmd(0x30); // 功能设定：基本指令集
    ssz_delay_us(200);
    lcd_write_cmd(0x0C); // 显示开，关光标
    ssz_delay_us(200);
    lcd_write_cmd(0x01);
    ssz_delay_us(20000);
    lcd_write_cmd(0x06);
    ssz_delay_us(100);

    uint8_t *line_1 = WORK_TIME_STR;
    uint8_t *line_2 = CUTTING_TIMES_STR;
    uint8_t *line_3 = TOTAL_TIME_STR;
    uint8_t *line_4 = "";

	lcd_write_msg(1, 0, line_1);
	lcd_write_msg(2, 0, line_2);
	lcd_write_msg(3, 0, line_3);
	lcd_write_msg(4, 0, line_4);
}


void lcd_check_busy(void)
{
    uint8_t status;
    uint16_t tmp;
    
    RS(0);
    RW(1);

    tmp = (GPIOB->ODR & 0xff00) | 0x00ff;
    GPIOB->ODR = tmp;
    lcd_data_port_init_input();

    do {
        EN(1);
        ssz_delay_us(5);
        status = HAL_GPIO_ReadPin(GPIOB, DB7_Pin);
        EN(0);
    } while (status);

    lcd_data_port_init_ouput();
}


void lcd_write_cmd(uint8_t cmd)
{
    uint16_t tmp;
    
    lcd_check_busy();
    RS(0);
    RW(0);
    ssz_delay_us(10);
    EN(1);
    ssz_delay_us(10);
    tmp = (GPIOB->ODR & 0xff00) | cmd;
    GPIOB->ODR = tmp;
    EN(0);
}


void lcd_write_data(uint8_t data)
{
    uint16_t tmp;
    
    lcd_check_busy();
    RS(1);
    RW(0);
    ssz_delay_us(5);
    tmp = (GPIOB->ODR & 0xff00) | data;
    GPIOB->ODR = tmp;
    EN(1);
    ssz_delay_us(10);
    EN(0);
}


void lcd_set_cursor(uint8_t row, uint8_t column)
{
    switch(row) {
    case 1:
        lcd_write_cmd(0x80 | column);
        break;
    case 2:
        lcd_write_cmd(0x90 | column);
        break;
    case 3:
        lcd_write_cmd(0x88 | column);
        break;
    case 4:
        lcd_write_cmd(0x98 | column);
        break;
    default:
        break;
    }
}


void lcd_write_msg(uint8_t row, uint8_t column, const uint8_t *message)
{
    // lcd_write_cmd(address);
    lcd_set_cursor(row, column);

    while (*message > 0) {
        lcd_write_data(*message);
        message++;
    }
}


void lcd_display_data(int32_t time, int32_t cutting_counter, int32_t total_time)
{
    char msg[32];

    memset(msg, 0, sizeof(msg));
    sprintf(msg, "%02d:%02d", time / 1000 / 60 % 100, time / 1000 % 60);
    lcd_write_msg(1, 5, (uint8_t *)msg);

    memset(msg, 0, sizeof(msg));
    sprintf(msg, "%04d", cutting_counter);
    lcd_write_msg(2, 5, (uint8_t *)msg);

    memset(msg, 0, sizeof(msg));
    total_time /= 1000;
    sprintf(msg, "%02d:%02d:%02d", total_time / 3600 % 100, total_time % 3600 / 60, total_time % 60);
    lcd_write_msg(3, 4, (uint8_t *)msg);
}

