#include <stdio.h>
#include "swusart.h"
#include "singlewire/single_wire_UART.h"

// internal
#define SWUSART_STATE_UNINITIALIZED  0
#define SWUSART_STATE_IDLE           1
#define SWUSART_STATE_WRITE_STARTBIT 2
#define SWUSART_STATE_WRITE_DATABIT  3
#define SWUSART_STATE_WRITE_STOPBIT  4

uint8_t state = SWUSART_STATE_UNINITIALIZED;

// events
void(*onStartBitDetected)() = NULL;

void(*onByteTransmitted)(uint8_t data) = NULL;

void(*onByteCorrupted)(uint8_t dataSent,
                       uint8_t dataReceived) = NULL;

// methods
void SWUSART_initialize(uint8_t mode,
                        uint32_t baud,
                        void(*startBitDetectedCallback)(),
                        void(*byteTransmittedCallback)(uint8_t data),
                        void(*byteCorruptedCallback)(uint8_t dataSent,
                                                     uint8_t dataReceived)) {

    onStartBitDetected = startBitDetectedCallback;
    onByteTransmitted = byteTransmittedCallback;
    onByteCorrupted = byteCorruptedCallback;

    // initialize hw usart
    state = SWUSART_STATE_IDLE;


    // set global interrupt enable
    //sei();
} // end of HWUSART_initialize

void SWUSART_writeByte (uint8_t data) {

	SW_UART_Enable();

    // trigger write for sw usart
	SW_UART_Transmit(data);

} // end of SWUSART_writeByte


//-----------
// internal -
//-----------

//ISR(SW_UART_EXTERNAL_INTERRUPT_VECTOR) {
//
//    //Make sure bit is low.
//    if(!READ_UART_PIN()) {
//	    onStartBitDetected();
//    } // end if
//} // end of SWUSART_externalISR
