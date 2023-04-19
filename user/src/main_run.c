
#include "version.h"
#include "event.h"
#include "ssz_common.h"
#include "com.h"
#include "timer.h"
#include "msg.h"
#include "drv_com.h"
#include "key.h"
#include "main.h"
#include "app_cmd.h"
#include <stdint.h>


/************************************************
* Declaration
************************************************/
#define TEST_TIME_5MINS_MS (5 * 60 * 1000)  // 5 minutes
// #define TEST_TIME_5MINS_MS (10 * 1000)

/************************************************
* Variable 
************************************************/
int32_t g_total_cutting_counter;
int32_t g_cutting_time;
int32_t g_total_cutting_time;
int32_t g_last_time;

/************************************************
* Function 
************************************************/
static void main_handle_pc_uart_recv()
{
    com_handle_new_received_data(kComPCUart);
}


static void main_init_event_handler()
{
    event_set_handler(kEventNewTick, timer_handle_all);
    event_set_handler(kEventPCUartReceiveNewData, main_handle_pc_uart_recv);
}


static void key_on_pressed(KeyID key, int repeat_count)
{
    g_last_time = ssz_tick_time_now();
    timer_start_periodic_every(kTimerCheck5min, 20);

    // printf("key pressed\n");
}

static void key_on_released(KeyID key, int repeat_count)
{
    int32_t time_ms;

    timer_stop(kTimerCheck5min);

    time_ms = ssz_tick_time_elapsed(g_last_time);
    g_total_cutting_counter++;
    g_cutting_time += time_ms;
    g_total_cutting_time += time_ms;

    printf("\nkey released, time:%ldms\ncurrent_time:%ldms, total_time:%ldms\ncutting counter:%ld\n",
        time_ms, g_cutting_time, g_total_cutting_time, g_total_cutting_counter);
}


static void reset_key_on_pressed(KeyID key, int repeat_count)
{
    g_total_cutting_counter = 0;
    g_cutting_time = 0;
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

    printf("\ncurrent_time:%ldms, total_time:%ldms\ncutting counter:%ld\n",
        g_cutting_time, g_total_cutting_time, g_total_cutting_counter);
}


void key_bind(void)
{
    key_set_handler(kKeyMax, kKeyEventPressed, key_on_pressed);
    key_set_handler(kKeyMax, kKeyEventReleased, key_on_released);

    key_set_handler(kKeyMin, kKeyEventPressed, key_on_pressed);
    key_set_handler(kKeyMin, kKeyEventReleased, key_on_released);

    key_set_handler(kKeyReset, kKeyEventPressed, reset_key_on_pressed);
}

/***********************************************
* Description:
*   handle all(e.g. event, timer, data receive...)
* Argument:
*
* Return:
*   if not handle anything, return false, otherwise return true
************************************************/
bool main_handle_all(void)
{
    bool ret = false;

    for (int i = 0; i < kEventIDMax; i++) {
        if (event_is_set((EventID)i)) {
            event_clear((EventID)i);
            event_handle((EventID)i);
            ret = true;
        }
    }

    return ret;
}

void buzzer_on(void)
{
  HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_SET);
}

void buzzer_off(void)
{
  HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_RESET);
}


uint16_t g_alarm_count;
uint16_t g_alarm_beep_times;

void beep_callback(void)
{
    if (g_alarm_count < g_alarm_beep_times) {
        buzzer_on();
        timer_set_handler(kTimerBuzzerOff, buzzer_off);
        timer_start_oneshot_after(kTimerBuzzerOff, 250);
        g_alarm_count++;
    } else {
      timer_stop(kTimerBeep);
    }
}

void beep(uint16_t times)
{
    g_alarm_count = 0;
    g_alarm_beep_times = times;
    beep_callback();
    timer_set_handler(kTimerBeep, beep_callback);
    timer_start_periodic_every(kTimerBeep, 500);
}


void check_if_reach_5mins(void)
{
    if (g_cutting_time + ssz_tick_time_elapsed(g_last_time) >= TEST_TIME_5MINS_MS) {
        beep(6);
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        timer_stop(kTimerCheck5min);
    }
}

/***********************************************
* Description:
*   main 
*
* Argument:
*
* Return:
*
************************************************/
void main_run(void)
{
    //init all modules
    // init_all();
    //init event handler

    drv_com_init();
    com_init_by_file(kComPCUart, drv_com_file(kComPCUart));
    msg_init();
    main_init_event_handler();
    key_init();
    app_cmd_init();
    key_start_scan();
    key_bind();

    timer_set_handler(kTimerCheck5min, check_if_reach_5mins);

    // ssz_traceln("%s power on.", version_software_name());
    printf("%s power on\n%s\n%s\n", version_software_name(), version_str(), version_build_time());

#ifdef TEST
    run_unit_test();
#endif

#if CPU_USAGE_MODULE_ENABLE
    cpu_usage_reset_usage();
#endif


    while(1) {
        //handle all
        if (main_handle_all() == false) {
            //if nothing need handle, go to sleep
            // ssz_sleep();
        }
    }
}
