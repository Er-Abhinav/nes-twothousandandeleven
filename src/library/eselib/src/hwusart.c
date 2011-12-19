#include <stdio.h>
#include <global.h>
#include "hwusart.h"

typedef enum hwusart_state {
	UNINITIALIZED = 0,
	IDLE,
	WRITE
} hwusart_state_t;


hwusart_state_t _state = UNINITIALIZED;
//uint8_t _state = HWUSART_STATE_UNINITIALIZED;

// events
void(*onByteReceived)(uint8_t data) = NULL;
void(*onByteTransmitted)(uint8_t data) = NULL;
void(*onByteCorrupted)(uint8_t dataSent, uint8_t dataReceived) = NULL;

// methods
void HWUSART_initialize(uint8_t mode, uint32_t baud,
		void(*byteReceivedCallback)(uint8_t data),
		void(*byteTransmittedCallback)(uint8_t data),
		void(*byteCorruptedCallback)(uint8_t dataSent, uint8_t dataReceived)) {

	onByteReceived = byteReceivedCallback;
	onByteTransmitted = byteTransmittedCallback;
	onByteCorrupted = byteCorruptedCallback;

	// initialize hw usart
	_state = IDLE;

	// setup baud rate
	uint16_t internal_baudrate = (CPU_CLK / (baud * 16)) - 1;

	UBRR1H = ((unsigned char) (internal_baudrate >> 8)) & 0x0F;
	UBRR1L = (unsigned char) internal_baudrate;

	UCSR1A = 0;

	// setup the connection
	UCSR1B = (1 << TXEN1) | (1 << RXEN1);

	// enable the interrupt
	UCSR1B |= (1 << RXCIE1);

	// set the connections' mode
	UCSR1C = 0;

	if (mode >= DOUBLESTOP) {
		UCSR1C |= (1 << USBS1);
	} /* end if */

	switch (mode) {
	case MODE_8N1:
		/* 8 databits, 1 stopbit, no parity */
		UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
		break;
	case MODE_8E1:
		/* 8 databits, 1 stopbit, even parity */
		UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
		UCSR1C |= (1 << UPM11);
		break;
	case MODE_8O1:
		/* 8 databits, 1 stopbit, odd parity */
		UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
		UCSR1C |= (1 << UPM11) | (1 << UPM10);
		break;

	case MODE_7N1:
		/* 7 databits, 1 stopbit, no parity */
		UCSR1C |= (1 << UCSZ11);
		break;
	case MODE_7E1:
		/* 7 databits, 1 stopbit, even parity */
		UCSR1C |= (1 << UCSZ11);
		UCSR1C |= (1 << UPM11);
		break;
	case MODE_7O1:
		/* 7 databits, 1 stopbit, odd parity */
		UCSR1C |= (1 << UCSZ11);
		UCSR1C |= (1 << UPM11) | (1 << UPM10);
		break;

	case MODE_6N1:
		/* 6 databits, 1 stopbit, no parity */
		UCSR1C |= (1 << UCSZ10);
		break;
	case MODE_6E1:
		/* 6 databits, 1 stopbit, even parity */
		UCSR1C |= (1 << UCSZ10);
		UCSR1C |= (1 << UPM11);
		break;
	case MODE_6O1:
		/* 6 databits, 1 stopbit, odd parity */
		UCSR1C |= (1 << UCSZ10);
		UCSR1C |= (1 << UPM11) | (1 << UPM10);
		break;

	case MODE_5N1:
		/* 5 databits, 1 stopbit, no parity */
		break;
	case MODE_5E1:
		/* 5 databits, 1 stopbit, even parity */
		UCSR1C |= (1 << UPM11);
		break;
	case MODE_5O1:
		/* 5 databits, 1 stopbit, odd parity */
		UCSR1C |= (1 << UPM11) | (1 << UPM10);
		break;

	default:
		/* do nothing */
		break;
	} /* end switch */

	// set global interrupt enable
	//sei();
} // end of HWUSART_initialize

void HWUSART_writeByte(uint8_t data) {

	// trigger write for hw usart

	// wait for empty transmit buffer
	while (!(UCSR1A & (1 << UDRE1)));

	// put data to send into buffer
	UDR1 = data;

	// should be called in the receive isr
	if (onByteTransmitted) {
		(*onByteTransmitted)(data);
	} // end if
} // end of HWUSART_writeByte


//-----------
// internal -
//-----------

/** \brief signal handler for receive interrupt of uart1 */
SIGNAL( SIG_USART1_RECV) {

	if (onByteReceived) {
		onByteReceived(UDR1);
	} // end if
} // end of SIGNAL(SIG_USART1_RECV)
