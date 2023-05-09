#include "drv_18b20.h"
// #include "tim.h"
#include "main.h"
#include "ssz_common.h"

// void DS18B20_Init(void)
// {
//     // RCC->APB2ENR |= 1 << 3;
//     // GPIOB->CRH &= 0x0FFFFFFF;
//     // GPIOB->CRH |= 0x30000000;
//     // GPIOB->ODR |= 1 << 15;
// }

const struct dev_port thermometer[] = {
    // {TEMP_GPIO_Port, TEMP_Pin},
    {wire_1_GPIO_Port, wire_1_Pin},
    {wire_2_GPIO_Port, wire_2_Pin},
};

void set_pin_mode_output(const struct dev_port *dev)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = dev->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(dev->port, &GPIO_InitStruct);
}


void set_pin_mode_input(const struct dev_port *dev)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = dev->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(dev->port, &GPIO_InitStruct);
}


uint8_t DS18B20_check_ack(const struct dev_port *dev)
{
    uint8_t cnt = 0;

    set_pin_mode_input(dev);
    while (HAL_GPIO_ReadPin(dev->port, dev->pin) && cnt < 200) {
        ssz_delay_us(4);
        cnt++;
    }
    if (cnt >= 200)
        return 1;

    cnt = 0;
    while ((!HAL_GPIO_ReadPin(dev->port, dev->pin)) && cnt < 240) {
        ssz_delay_us(4);
        cnt++;
    }
    if (cnt >= 240)
        return 1;
    return 0;
}


uint8_t ds18b20_check_device(const struct dev_port *dev) //包含了复位脉冲、检测存在脉冲
{

    set_pin_mode_output(dev);
    HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_RESET);
    ssz_delay_us(800);
    HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);
    ssz_delay_us(30);

    if (DS18B20_check_ack(dev)) {
        return 1;
    }

    return 0;
}


void ds18b20_write_byte(const struct dev_port *dev, uint8_t cmd)
{
    uint8_t i;

    set_pin_mode_output(dev);

    for (i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_RESET);
        ssz_delay_us(4);
        HAL_GPIO_WritePin(dev->port, dev->pin, (GPIO_PinState)(cmd & 0x01));
        ssz_delay_us(60); 
        HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);
        cmd >>= 1;
    }
}


uint8_t ds18b20_read_byte(const struct dev_port *dev)
{
    uint8_t i, data = 0;

    for (i = 0; i < 8; i++) {
        set_pin_mode_output(dev);
        HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_RESET);
        ssz_delay_us(4);
        HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);

        set_pin_mode_input(dev);
        ssz_delay_us(10);

        data >>= 1;
        if (HAL_GPIO_ReadPin(dev->port, dev->pin))
            data |= 0x80;

        ssz_delay_us(60);
        set_pin_mode_output(dev);
        HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);
    }

    return data;
}


uint16_t ds18b20_read_temp(const struct dev_port *dev)
{
    uint16_t temp = 0;
    uint8_t temp_H = 0, temp_L = 0;
    uint32_t tick_start = 0U;

    ds18b20_check_device(dev);
    ds18b20_write_byte(dev, 0xCC);
    ds18b20_write_byte(dev, 0x44);

    tick_start = HAL_GetTick();
    // wait conversion completed
    while (1) {
        if (ds18b20_read_byte(dev) == 0xFF || (HAL_GetTick() - tick_start) > 200)
            break;
    }

    ds18b20_check_device(dev);
    ds18b20_write_byte(dev, 0xCC);
    ds18b20_write_byte(dev, 0xBE);

    temp_L = ds18b20_read_byte(dev);
    temp_H = ds18b20_read_byte(dev);
    temp = temp_L | (temp_H << 8);

    return temp;
}

uint16_t read_channel_temp(enum tmp_channel_t ch)
{
    // printf("temp:%u\n", ds18b20_read_temp(&thermometer[0]));
    return ds18b20_read_temp(&thermometer[ch]);
}


