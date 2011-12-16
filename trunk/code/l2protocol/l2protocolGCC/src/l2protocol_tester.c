#include "../inc/l1interface.h"
#include "../inc/l1testing.h"
#include "../inc/l2protocol.h"

#include <stdio.h>
#include <string.h>

void __testMessageReceive();
void __testMessageTransmit();

void __testMessageTransmitWhileReceive();
void __testMessageReceiveWithSenderAbort();
void __testMessageReceiveWithWrongCRC();

void transmittedCallback(l2Msg_t msg);

int main(int argc, char **argv)
{

	l2Initialize(NULL,NULL,transmittedCallback,NULL,NULL,NULL);


	//l2MsgId_t msgId;


	__testMessageTransmit();


	l2Close();

	return 0;
}

void __testMessageReceive()
{
	uint8_t i;

	for(i=0;i<40; i++) {
		__onHWUSARTByteReceived(i);
	}
}

void __testMessageTransmit()
{

	l2Msg_t msg;

	msg.msgId.full = 1234;
	strcpy(msg.payload, (uint8_t*)"Hallo!");
	msg.payloadLen = strlen("Hallo!");

	l2WriteMessage(msg);
}

void transmittedCallback(l2Msg_t msg)
{

}

void __testMessageTransmitWhileReceive()
{

}

void __testMessageReceiveWithSenderAbort()
{

}

void __testMessageReceiveWithWrongCRC()
{

}
