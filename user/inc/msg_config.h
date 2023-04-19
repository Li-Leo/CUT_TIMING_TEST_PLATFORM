#pragma once

/************************************************
* Declaration
************************************************/
#define MSG_QUEUE_MAX_NUM 20

typedef enum {
	kMsgNull,
	// kMsgAlarmSet,
	// kMsgAlarmClear,
	kMsgReachLifetimeLimit,

    kMsgIDMax
}MsgID;
	
