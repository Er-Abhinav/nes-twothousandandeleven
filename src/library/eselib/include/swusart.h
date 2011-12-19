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

/***********/
/* defines */
/***********/
/** \brief begin of constants for modes with one stop bit */
#define SINGLESTOP 0

/** \brief 8 databits, 1 stoppbit, no parity */
#define MODE_8N1  0 // 8 Datenbits, 1 Stoppbit, kein Paritycheck
/** \brief 8 databits, 1 stoppbit, even parity */
#define MODE_8E1  1 // 8 Datenbits, 1 Stoppbit, even parity
/** \brief 8 databits, 1 stoppbit, odd parity */
#define MODE_8O1  2 // 8 Datenbits, 1 Stoppbit, odd parity

/** \brief 7 databits, 1 stoppbit, no parity */
#define MODE_7N1  3 // 7 Datenbits, 1 Stoppbit, kein Paritycheck
/** \brief 7 databits, 1 stoppbit, even parity */
#define MODE_7E1  4 // 7 Datenbits, 1 Stoppbit, even parity
/** \brief 7 databits, 1 stoppbit, odd parity */
#define MODE_7O1  5 // 7 Datenbits, 1 Stoppbit, odd parity

/** \brief 6 databits, 1 stoppbit, no parity */
#define MODE_6N1  6 // 6 Datenbits, 1 Stoppbit, kein Paritycheck
/** \brief 6 databits, 1 stoppbit, even parity */
#define MODE_6E1  7 // 6 Datenbits, 1 Stoppbit, even parity
/** \brief 6 databits, 1 stoppbit, odd parity */
#define MODE_6O1  8 // 6 Datenbits, 1 Stoppbit, odd parity

/** \brief 5 databits, 1 stoppbit, no parity */
#define MODE_5N1  9 // 5 Datenbits, 1 Stoppbit, kein Paritycheck
/** \brief 5 databits, 1 stoppbit, even parity */
#define MODE_5E1 10 // 5 Datenbits, 1 Stoppbit, even parity
/** \brief 5 databits, 1 stoppbit, odd parity */
#define MODE_5O1 11 // 5 Datenbits, 1 Stoppbit, odd parity

/** \brief begin of constants for modes with two stop bit */
#define DOUBLESTOP 20

/** \brief 8 databits, 2 stoppbit, no parity */
#define MODE_8N2 (DOUBLESTOP+MODE_8N1) // 8 Datenbits, 2 Stoppbit, kein Paritycheck
/** \brief 8 databits, 2 stoppbit, even parity */
#define MODE_8E2 (DOUBLESTOP+MODE_8E1) // 8 Datenbits, 2 Stoppbit, even parity
/** \brief 8 databits, 2 stoppbit, odd parity */
#define MODE_8O2 (DOUBLESTOP+MODE_8O1) // 8 Datenbits, 2 Stoppbit, odd parity

/** \brief 7 databits, 2 stoppbit, no parity */
#define MODE_7N2 (DOUBLESTOP+MODE_7N1) // 7 Datenbits, 2 Stoppbit, kein Paritycheck
/** \brief 7 databits, 2 stoppbit, even parity */
#define MODE_7E2 (DOUBLESTOP+MODE_7E1) // 7 Datenbits, 2 Stoppbit, even parity
/** \brief 7 databits, 2 stoppbit, odd parity */
#define MODE_7O2 (DOUBLESTOP+MODE_7O1) // 7 Datenbits, 2 Stoppbit, odd parity

/** \brief 6 databits, 2 stoppbit, no parity */
#define MODE_6N2 (DOUBLESTOP+MODE_6N1) // 6 Datenbits, 2 Stoppbit, kein Paritycheck
/** \brief 6 databits, 2 stoppbit, even parity */
#define MODE_6E2 (DOUBLESTOP+MODE_6E1) // 6 Datenbits, 2 Stoppbit, even parity
/** \brief 6 databits, 2 stoppbit, odd parity */
#define MODE_6O2 (DOUBLESTOP+MODE_6O1) // 6 Datenbits, 2 Stoppbit, odd parity

/** \brief 5 databits, 2 stoppbit, no parity */
#define MODE_5N2 (DOUBLESTOP+MODE_5N1) // 5 Datenbits, 2 Stoppbit, kein Paritycheck
/** \brief 5 databits, 2 stoppbit, even parity */
#define MODE_5E2 (DOUBLESTOP+MODE_5E1) // 5 Datenbits, 2 Stoppbit, even parity
/** \brief 5 databits, 2 stoppbit, odd parity */
#define MODE_5O2 (DOUBLESTOP+MODE_5O1) // 5 Datenbits, 2 Stoppbit, odd parity

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
                        void(*startBitDetectedCallback)()
                        );

/**
* \brief transmit a single character (blocking mode)
* \param byte the character to be transmitted
* \return none
*/
void SWUSART_writeByte (uint8_t data);

#endif
