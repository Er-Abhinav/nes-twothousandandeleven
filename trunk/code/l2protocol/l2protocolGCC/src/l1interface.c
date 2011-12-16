#include <stdio.h>
#include <stdint.h>

#include "../inc/l1interface.h"
#include "../inc/l1testing.h"

// events
void(*onHWUSARTByteReceived)(uint8_t data) = NULL;

void(*onHWUSARTByteTransmitted)(uint8_t data) = NULL;

void(*onHWUSARTByteCorrupted)(uint8_t dataSent,
                       uint8_t dataReceived) = NULL;

// methods
void HWUSART_initialize(uint8_t mode,
                        uint32_t baud,
                        void(*byteReceivedCallback)(uint8_t data),
                        void(*byteTransmittedCallback)(uint8_t data),
                        void(*byteCorruptedCallback)(uint8_t dataSent,
                                                     uint8_t dataReceived)) {

    onHWUSARTByteReceived = byteReceivedCallback;
    onHWUSARTByteTransmitted = byteTransmittedCallback;
    onHWUSARTByteCorrupted = byteCorruptedCallback;

}

void HWUSART_enable(){}
void HWUSART_disable(){}

void HWUSART_writeByte (uint8_t data) {
	// choose method of choice
	__onHWUSARTByteTransmitted(data);
}

// events
void(*onSWUSARTStartBitDetected)() = NULL;

void(*onSWUSARTByteTransmitted)(uint8_t data) = NULL;

void(*onSWUSARTByteCorrupted)(uint8_t dataSent,
                       uint8_t dataReceived) = NULL;

// methods
void SWUSART_initialize(uint8_t mode,
                        uint32_t baud,
                        void(*startBitDetectedCallback)(),
                        void(*byteTransmittedCallback)(uint8_t data),
                        void(*byteCorruptedCallback)(uint8_t dataSent,
                                                     uint8_t dataReceived)) {

    onSWUSARTStartBitDetected = startBitDetectedCallback;
    onSWUSARTByteTransmitted = byteTransmittedCallback;
    onSWUSARTByteCorrupted = byteCorruptedCallback;
}


void SWUSART_enable(){}
void SWUSART_disable(){}

void SWUSART_writeByte (uint8_t data) {

	// choose method of choice
	__onSWUSARTByteTransmitted(data);
}




void __onSWUSARTStartBitDetected() {
	if(onSWUSARTStartBitDetected)
		onSWUSARTStartBitDetected();
}

void __onSWUSARTByteTransmitted(uint8_t data) {
	if(onSWUSARTByteTransmitted)
		onSWUSARTByteTransmitted(data);
}

void __onSWUSARTByteCorrupted(uint8_t dataSent,
                              uint8_t dataReceived) {
	if(onSWUSARTByteCorrupted)
		onSWUSARTByteCorrupted(dataSent, dataReceived);
}

void __onHWUSARTByteReceived(uint8_t data) {
	if(onHWUSARTByteReceived)
		onHWUSARTByteReceived(data);
}

void __onHWUSARTByteTransmitted(uint8_t data) {
	if(onHWUSARTByteTransmitted)
		onHWUSARTByteTransmitted(data);
}

void __onHWUSARTByteCorrupted(uint8_t dataSent,
                              uint8_t dataReceived){
	if(onHWUSARTByteCorrupted)
		onHWUSARTByteCorrupted(dataSent, dataReceived);
}
