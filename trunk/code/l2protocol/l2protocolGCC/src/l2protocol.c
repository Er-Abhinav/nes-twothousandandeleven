#include "../inc/l2protocol.h"
#include "../inc/l2config.h"
#include "../inc/l2error.h"
#include "../lib/queue/queue.h"
#include "../lib/stuffing/stuffing.h"

#include "../inc/l1interface.h" // replace with real hw and sw usart implementation
#include <stdio.h>
#include <string.h>


typedef enum l2MsgState {
	PROCESSED,
	UNPROCESSED
} l2MsgState_t;

// internal prototypes
uint8_t __transmitNextMessage();
uint8_t __transmitMessage(l2Msg_t msg);

// events
void(*onMessageReceived)(l2Msg_t msg) = NULL;

void(*onMessagePreTransmit)(l2Msg_t msg) = NULL;

void(*onMessageTransmitted)(l2Msg_t msg) = NULL;

void(*onMessageTransmitError)(l2Msg_t msg) = NULL;

void(*onReceiveQueueOverflow)(void) = NULL;

void(*onSendQueueOverflow)(void) = NULL;

// l2 event triggering

// events
inline void __triggerOnMessageReceived(l2Msg_t msg) {
	if(onMessageReceived)
		onMessageReceived(msg);
} // end of __onMessageReceived

inline void __triggerOnMessageTransmitted(l2Msg_t msg) {
	if(onMessageTransmitted)
		onMessageTransmitted(msg);
} // end of __onMessageTransmitted

inline void __triggerOnMessagePreTransmit(l2Msg_t msg) {
	if(onMessagePreTransmit)
		onMessagePreTransmit(msg);
} // end of __onMessagePreTransmit

inline void __triggerOnMessageTransmitError(l2Msg_t msg) {
	if(onMessageTransmitError)
		onMessageTransmitError(msg);
} // end of __onMessageTransmitError

inline void __triggerOnReceiveQueueOverflow() {
	if(onReceiveQueueOverflow)
		onReceiveQueueOverflow();
} // end of __onReceiveQueueOverflow

inline void __triggerOnSendQueueOverflow() {
	if(onSendQueueOverflow)
		onSendQueueOverflow();
} // end of __onSendQueueOverflow


// l1 event handler
void __byteReceivedEventHandler(uint8_t data);

void __byteTransmittedEventHandler (uint8_t data);

void __byteCorruptedEventHandler(uint8_t dataSent,
		  					     uint8_t dataReceived);

void __startBitDetectedEventHandler();

void __arbitrationSucceedEventHandler(uint8_t data);

void __arbitrationFailedEventHandler(uint8_t dataSent,
                                     uint8_t dataReceived);



// interface implementation
l2State_t l2state = UNINITIALIZED;

queue receiveQueue;
queue sendQueue;

uint8_t l2Initialize(void(*messageReceivedCallback)(l2Msg_t msg),
  		             void(*messagePreTransmitCallback)(l2Msg_t msg),
                     void(*messageTransmittedCallback)(l2Msg_t msg),
                     void(*messageTransmitErrorCallback)(l2Msg_t msg),
                     void(*receiveQueueOverflowCallback)(void),
                     void(*sendQueueOverflowCallback)(void)) {

	onMessageReceived = messageReceivedCallback;
	onMessagePreTransmit = messagePreTransmitCallback;
	onMessageTransmitted = messageTransmittedCallback;
	onMessageTransmitError = messageTransmitErrorCallback;
	onReceiveQueueOverflow = receiveQueueOverflowCallback;
	onSendQueueOverflow = sendQueueOverflowCallback;

	// TODO: (alex) fix me
	//l2state = STARTUP;
	l2state = IDLE;
	sendMsgState = UNPROCESSED;

	// initialize sublayers
	HWUSART_initialize((uint8_t)L2CONFIG_UART_MODE,
					   (uint32_t)L2CONFIG_UART_BAUD,
				       __byteReceivedEventHandler,
				       __byteTransmittedEventHandler,
				       __byteCorruptedEventHandler);

	SWUSART_initialize((uint8_t)L2CONFIG_UART_MODE,
			           (uint32_t)L2CONFIG_UART_BAUD,
			           __startBitDetectedEventHandler,
			           __arbitrationSucceedEventHandler,
			           __arbitrationFailedEventHandler);

	// initialize send and receive queues
    queue_init(&receiveQueue, (uint8_t)L2CONFIG_RECEIVEQUEUE_LEN);
    queue_init(&sendQueue, (uint8_t)L2CONFIG_SENDQUEUE_LEN);

	return L2ERROR_SUCCESS;
} // end of l2Initialize

uint8_t l2Close () {

	// close l1

	//	if(HWUSART_close() != L1ERROR_SUCCESS)
	//		return L2ERROR_CLOSEL1FAILED;
	//
	//	if(SWUSART_close() != L1ERROR_SUCCESS)
	//		return L2ERROR_CLOSEL1FAILED;


	// close queues
	queue_close(&receiveQueue);
	queue_close(&sendQueue);


	l2state = UNINITIALIZED;

	return L2ERROR_SUCCESS;
}

uint8_t l2WriteMessage(l2Msg_t msg) {
	return l2WriteMessageAccordingToPrio(msg, NORMAL);
}

uint8_t l2WriteMessageImmediate(l2Msg_t msg) {
	return l2WriteMessageAccordingToPrio(msg, IMMEDIATE);
}

uint8_t l2WriteMessageAccordingToPrio(l2Msg_t msg, l2MsgPrio_t prio) {
    uint8_t err = L2ERROR_SUCCESS;

    if(l2state != IDLE) {
		// write to transmit queue
		queue_entry qe;
		qe.msgType = msg.msgId.full;
		qe.msgSize = msg.payloadLen;
		memcpy(qe.msgPayload, msg.payload, msg.payloadLen * sizeof(uint8_t));

		queue_append(&sendQueue, &qe);
    }
    else
    {
    	err = __transmitMessage(msg);
    } // end if

	return err;
}

uint8_t __transmitNextMessage() {
    uint8_t err = L2ERROR_SUCCESS;
	queue_entry qe;
	l2Msg_t msg;

	if(sendMsgState == PROCESSED) {

		if(!queue_isEmpty(&sendQueue)) {
			// read from queue
			queue_getNext(&sendQueue, &qe);

			msg.msgId.full = qe.msgType;
			msg.payloadLen = qe.msgSize;
			memcpy(msg.payload, qe.msgPayload, qe.msgSize * sizeof(uint8_t));

			err = __transmitMessage(msg);
		}
		else {
			l2state = IDLE;
		} // end if
	}
	else if (sendMsgState == UNPROCESSED){
		__transmitMessage(sendMsg);
	} // end if

	return err;
}

l2MsgState_t sendMsgState = UNPROCESSED;

uint8_t sendBufferBytesTransmitted = 0;
uint8_t sendBufferBytesInUse = 0;
// 3 header + 16 payload + 4 crc + 6 worst case stuff bytes
uint8_t sendBuffer[3+16+4+6];

l2Msg_t sendMsg;

uint8_t __transmitMessage(l2Msg_t msg) {

    uint8_t i,j;

    uint8_t outbufferBytesInUse = 0;
    // 3 header + 16 payload + 4 crc
    uint8_t outbuffer[3+16+4];


    // copy original message
    memcpy(sendMsg.payload,
    	   msg.payload,
    	   msg.payloadLen * sizeof(uint8_t));
    sendMsg.payloadLen = msg.payloadLen;
    sendMsg.msgId = msg.msgId;

    // build transmit buffer
    ((uint32_t*)outbuffer)[0] = (((uint32_t)msg.msgId.full)<<12) | (msg.payloadLen & 0x0F);
    for(i=0, j=4; i<msg.payloadLen;i++, j++) {
    	outbuffer[j] = msg.payload[i];
    }

    // TODO: (alex) adapt for crc checksum
    outbufferBytesInUse = 3 + msg.payloadLen; // + 4 crc

    // TODO: (alex) adapt for bit stuffing
    //stuff()
    for(i=0; i<outbufferBytesInUse; i++) {        // set by stuff();
    	sendBuffer[i] = outbuffer[i];			  // set by stuff();
    }											  // set by stuff();

    sendBufferBytesInUse = outbufferBytesInUse;   // set by stuff();
    sendBufferBytesTransmitted = 0;

    // ARBITRATE
    l2state = ARBITRATE;

    // switch off hw usart
    HWUSART_disable();

    // start arbitration
    SWUSART_enable();
    SWUSART_writeByte(sendBuffer[sendBufferBytesTransmitted]);

	return L2ERROR_SUCCESS;
} // end of __transmitMessage

// l1 event handler implementation

uint8_t receiveBufferBytesInUse = 0;
uint8_t receiveBufferBytesReceived = 0;
// 3 header + 16 payload + 4 crc + 6 worst case stuff bytes
uint8_t receiveBuffer[3+16+4+6];

l2Msg_t receiveMsg;

void __byteReceivedEventHandler(uint8_t data) {
	uint8_t i;

	if(!(data & 0xf0)) {
		// SOF detected
		receiveBufferBytesReceived = 0;
		receiveBufferBytesInUse = 0;
		l2state = RECEIVE;
	} // end if


	if(l2state == IDLE) {
		receiveBufferBytesReceived = 0;
		receiveBufferBytesInUse = 0;

		l2state = RECEIVE;
	}

	if(l2state == RECEIVE) {
		receiveBuffer[receiveBufferBytesReceived] = data;
		receiveBufferBytesReceived++;

		// TODO: (alex) has to be adapted for bit unstuffing!!!
		if(receiveBufferBytesInUse == 0) {
			if(receiveBufferBytesReceived >= 3) {

				receiveBufferBytesInUse = (receiveBuffer[3] & 0x0f);
			}
		}

		// TODO: (alex) adapt with crc checksum length
		// check message has finished
		if(receiveBufferBytesReceived >= receiveBufferBytesInUse + 3) { // +4 for crc
			// TODO: (alex) unstuff msg

			// build receiveMsg from receiveBuffer
			receiveMsg.msgId.type = receiveBuffer[0] & 0x0f;
			receiveMsg.msgId.receiver = (receiveBuffer[1] & 0xfc) >> 2;
			receiveMsg.msgId.sender = (receiveBuffer[1] & 0x03 << 4) | (receiveBuffer[2] & 0xf0 >> 4);

			receiveMsg.payloadLen = receiveBufferBytesInUse;

			for(i=0; i<receiveMsg.payloadLen; i++) {
				receiveMsg.payload[i] = receiveBuffer[i+3];
			}

			receiveBufferBytesInUse = 0;
			receiveBufferBytesReceived = 0;

			__triggerOnMessageReceived(receiveMsg);
			// TODO: (alex) add message to queue

			__transmitNextMessage();
		}
	}
}

void __byteTransmittedEventHandler (uint8_t data) {

	if(l2state == SEND)
	{
		if(data == sendBuffer[sendBufferBytesTransmitted]) {
			sendBufferBytesTransmitted++;

			if(sendBufferBytesTransmitted < sendBufferBytesInUse) {
				HWUSART_writeByte(sendBuffer[sendBufferBytesTransmitted]);
			}
			else {
				// already finished
				sendBufferBytesInUse = 0;
				sendBufferBytesTransmitted = 0;

				l2MsgState = PROCESSED;
				l2state = IDLE;

			    // trigger transmission finished event
			    __triggerOnMessageTransmitted(sendMsg);
			    __transmitNextMessage();
			} // end if
		}
	}
}

void __byteCorruptedEventHandler(uint8_t dataSent,
							     uint8_t dataReceived) {
	__triggerOnMessageTransmitError(sendMsg);
	l2state = IDLE;

	__transmitNextMessage();
}

void __startBitDetectedEventHandler() {
}

void __arbitrationSucceedEventHandler(uint8_t data) {
	if(l2state == ARBITRATE) {
		if(data == sendBuffer[sendBufferBytesTransmitted]) {
			if(sendBufferBytesTransmitted >= 3) {
				// arbitration succeeded bus is ours
				SWUSART_disable();
				HWUSART_enable();

				l2state = SEND;

				sendBufferBytesTransmitted++;

				if(sendBufferBytesTransmitted < sendBufferBytesInUse) {
					HWUSART_writeByte(sendBuffer[sendBufferBytesTransmitted]);
				}
				else {
					// already finished
					sendBufferBytesInUse = 0;
				    sendBufferBytesTransmitted = 0;

				    // trigger transmission finished event
				    __triggerOnMessageTransmitted(sendMsg);
				    __transmitNextMessage();
				} // end if
			}

			sendBufferBytesTransmitted++;
			SWUSART_writeByte(sendBuffer[sendBufferBytesTransmitted]);
		} // end if
	} // end if
}

void __arbitrationFailedEventHandler(uint8_t dataSent,
                                     uint8_t dataReceived) {
	uint8_t i;

	if(l2state == ARBITRATE) {

		for(i=0; i<sendBufferBytesTransmitted; i++) {
			receiveBuffer[i] = sendBuffer[i]
		}
		receiveBuffer[i] = dataReceived;

		l2state = RECEIVE;

		sendBufferBytesTransmitted = 0;

		SWUSART_disable();
		HWUSART_enable();
	}
}
