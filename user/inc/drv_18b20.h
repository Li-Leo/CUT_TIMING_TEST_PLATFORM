#ifndef DS18B20_H
#define DS18B20_H

#include "stdint.h"
#include "gpio.h"
 
 
struct dev_port
{
    GPIO_TypeDef *port;
    uint16_t pin;
};

enum tmp_channel_t
{
    kChannel1,  // wire_1
    kChannel2,

    kChannelMax
};

extern const struct dev_port thermometer[];
uint16_t ds18b20_read_temp(const struct dev_port *dev);
uint16_t read_channel_temp(enum tmp_channel_t ch);
 
#endif
