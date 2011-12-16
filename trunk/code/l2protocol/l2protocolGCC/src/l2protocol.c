#include "../inc/l2protocol.h"
#include "../inc/l2config.h"
#include "../inc/l2error.h"
#include "../lib/queue/queue.h"
#include "../lib/stuffing/stuffing.h"

#include "../inc/l1interface.h" // replace with real hw and sw usart implementation
#include <stdio.h>
#include <string.h>

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

	if(!queue_isEmpty(&sendQueue)) {
    	// read from queue
    	queue_getNext(&sendQueue, &qe);

    	msg.msgId.full = qe.msgType;
    	msg.payloadLen = qe.msgSize;
    	memcpy(msg.payload, qe.msgPayload, qe.msgSize * sizeof(uint8_t));

    	err = __transmitMessage(msg);
    } // end if

	return err;
}

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

    outbufferBytesInUse = 3 + msg.payloadLen; // + 4 crc

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
void __byteReceivedEventHandler(uint8_t data) {

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

			    // trigger transmission finished event
			    __triggerOnMessageTransmitted(sendMsg);
			    __transmitNextMessage();
			} // end if
		}
	}
}

void __byteCorruptedEventHandler(uint8_t dataSent,
							   uint8_t dataReceived) {

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


}
