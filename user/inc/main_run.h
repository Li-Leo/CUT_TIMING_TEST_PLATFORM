#pragma once

/************************************************
* Declaration
************************************************/
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


void main_run(void);
bool main_handle_all(void);

void cut_delay_on(void);
void cut_delay_off(void);

void led_on(void);
void led_off(void);

void beep(uint16_t times);

#ifdef __cplusplus
}
#endif

