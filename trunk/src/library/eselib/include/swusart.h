/**
* \file hwusart.h
* Name: SW USART Driver\n
* \brief driver module for hardware usart \n
* \author Alexander Heinisch (0627820)
* \date 02.12.2011
*/

#ifndef SWUSART_H
#define SWUSART_H

/************/
/* includes */
/************/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "single_wire_UART.h"

typedef enum _sw_uart_state_e {
	UNINITIALIZED = -1,
	DISABLED = 0,
	ENABLED,
	WRITE_STARTBIT,
	WRITE_DATA,
	READ_DATA,
	WRITE_STOP,
	FINISH
} _sw_uart_state_t;

typedef enum swUart_finish_states {
	UNKNOWN = -1,
	BUSY,
	GOT_BUS,
	ARBITRATION_LOST
} swUart_finish_state_t;

/**
* \brief initializes the uart
* \param mode specifies the used usart message configuration
* \param baud specifies the used baudrate
* \param startBitDetectedCallback is the event triggered when a start bit has been detected
* \param byteTransmittedCallback is the data transmitted event callbackroutine
* \param byteCorruptedCallback is the data corrupted event callbackroutine
* \return none
*/

void SWUSART_initialize(uint8_t mode,
                        uint32_t baud,
                        void(*startBitDetectedCallback)(void));

/**
* \brief transmit a single character (blocking mode)
* \param byte the character to be transmitted
* \return none
*/
void SWUSART_writeByte (uint8_t data);

int8_t swUart_arbitrate(uint8_t data);

int8_t    SW_UART_Enable(void);       //!< Enable the UART.
void    SW_UART_Disable(void);      //!< Disable the UART.

#endif
