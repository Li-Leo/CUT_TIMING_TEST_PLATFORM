#pragma once
/************************************************
* Declaration
************************************************/
#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	kEventNewTick,
	kEventReceiveNewMsg,
	kEventPCUartReceiveNewData,

	kEventIDMax
} EventID;

#ifdef __cplusplus
}
#endif


