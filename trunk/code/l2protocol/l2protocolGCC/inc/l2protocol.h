#ifndef L2PROTOCOL_H
#define L2PROTOCOL_H

#include <stdint.h>


//typedef struct l2MsgId
//{
//	unsigned type : 4;
//	unsigned receiver : 6;
//	unsigned sender : 6;
//} l2MsgId_t;

typedef union l2MsgId
{
	struct{
		unsigned type : 4;
	    unsigned receiver : 6;
	    unsigned sender : 6;
	};
	uint16_t full;
} l2MsgId_t;


typedef struct l2Msg
{
	l2MsgId_t msgId;
	uint8_t   payload[16];
	uint8_t   payloadLen;
} l2Msg_t;

typedef enum l2MsgPrio {
	NORMAL,
	IMMEDIATE
} l2MsgPrio_t;

typedef enum l2State {
	UNINITIALIZED,
	STARTUP,
	IDLE,
	ARBITRATE,
	SEND,
	RECEIVE
} l2State_t;

uint8_t l2Initialize(void(*messageReceivedCallback)(l2Msg_t msg),
					 void(*messagePreTransmitCallback)(l2Msg_t msg),
					 void(*messageTransmittedCallback)(l2Msg_t msg),
					 void(*messageTransmitErrorCallback)(l2Msg_t msg),
					 void(*receiveQueueOverflowCallback)(void),
					 void(*sendQueueOverflowCallback)(void));

uint8_t l2Close ();

uint8_t l2WriteMessage(l2Msg_t msg);

uint8_t l2WriteMessageImmediate(l2Msg_t msg);

uint8_t l2WriteMessageAccordingToPrio(l2Msg_t msg, l2MsgPrio_t prio);

#endif
