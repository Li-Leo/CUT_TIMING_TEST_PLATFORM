#pragma once

/************************************************
* Declaration
************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#define TIMER_GROUP_NORMAL 0x01
// the timer id is relate with screen, So when exit screen, the id will auto stop
#define TIMER_GROUP_SCREEN 0x02 
#define TIMER_GROUP_LOW_POWER_MODE 0x04
#define TIMER_GROUP_ALL 0xFF

#define low_power_timer_set_handler(id, func) timer_set_handler_and_group(id, func, TIMER_GROUP_LOW_POWER_MODE)

typedef enum {

    kTimerScanKey,
    kTimerRepeatCmd,

    kTimerCheck5min,
    kTimerBeep,
    kTimerBuzzerOff,

    // kTimerRepeatCmd,

    kTimerIDMax
}TimerID;

#ifdef __cplusplus
}
#endif


