#include "lcd.h"

void ssz_delay_ms(int ms);

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

    lcd_write_cmd(0x30); // 功能设定：基本指令集
    ssz_delay_ms(5);
    lcd_write_cmd(0x0C); // 显示开，关光标
    ssz_delay_ms(5);
    lcd_write_cmd(0x01); // 清除显示
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
        ssz_delay_ms(5);
        status = HAL_GPIO_ReadPin(GPIOB, DB7_Pin);
    } while (status & 0x80);

    EN(0);
    lcd_data_port_init_ouput();
}


void lcd_write_cmd(uint8_t cmd)
{
    uint16_t tmp;
    
    lcd_check_busy();
    RS(0);
    RW(0);
    ssz_delay_ms(5);
    tmp = (GPIOB->ODR & 0xff00) | cmd;
    GPIOB->ODR = tmp;
    EN(1);
    ssz_delay_ms(10);
    EN(0);
}


void lcd_write_data(uint8_t data)
{
    uint16_t tmp;
    
    lcd_check_busy();
    RS(1);
    RW(0);
    ssz_delay_ms(5);
    tmp = (GPIOB->ODR & 0xff00) | data;
    GPIOB->ODR = tmp;
    EN(1);
    ssz_delay_ms(10);
    EN(0);
}


void lcd_write_msg(uint8_t *message, uint8_t address)
{
    lcd_write_cmd(address);

    while (*message > 0) {
        lcd_write_data(*message);
        message++;
    }
}
