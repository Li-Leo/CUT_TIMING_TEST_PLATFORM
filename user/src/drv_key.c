#include "drv_key.h"
#include "main.h"

/************************************************
* Declaration
************************************************/

/************************************************
* Variable 
************************************************/
struct key_config
{
    GPIO_TypeDef* gpio_port;
    uint16_t gpio_pin;
    bool pressed_state;
};


const static struct key_config g_key_gpios[kKeyIDMax] =
{
    {MAX_GPIO_Port, MAX_Pin, false},
    {MIN_GPIO_Port, MIN_Pin, false},
    {WORK_RST_GPIO_Port, WORK_RST_Pin, false},
};

/************************************************
* Function 
************************************************/
bool gpio_is_valid(const struct key_config * key)
{
    bool ret;
   
    ret = (HAL_GPIO_ReadPin(key->gpio_port, key->gpio_pin) == key->pressed_state) ? true : false;
    
    return ret;
}

//it will scan all key state and put at the param
void drv_key_scan(int8_t key_state[kKeyIDMax])
{
    for (int i = 0; i < kKeyIDMax; ++i) {
        key_state[i] = gpio_is_valid(&g_key_gpios[i]);
    }
}

// bool drv_key_is_pressed(KeyID key)
// {
//     // return ssz_gpio_is_valid(&g_key_gpios[key]);
// }

// bool drv_key_is_high(KeyID key)
// {
// 	// return ssz_gpio_is_high(g_key_gpios[key].gpio, g_key_gpios[key].pin_mask);
// }

