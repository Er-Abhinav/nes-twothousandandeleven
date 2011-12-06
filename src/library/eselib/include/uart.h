#ifndef __UART_H__
#define __UART_H__

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <global.h>
#include <ringBuff.h>

/** \file uart.h
 * \brief Header for the uart driver
 * \author Martin Perner, 0725782
 * \version 0.9
 * \date 2009/10/29
 *
 * Allows transmitting and receiving from/to uart0/1
 */

#if !defined CPU_CLK
	#error "Timer-Values are set for a different clock"
#endif

/**
 * \brief Maximum value for UBRR
 */
#define UART_MAX_UBRR 4095

#define UART_PARITY_NONE 0x00 //0b00000000
#define UART_PARITY_EVEN 0x20 //0b00100000
#define UART_PARITY_ODD  0x30 //0b00110000

#define UART_STOPBIT_1   0x00 //0b00000000
#define UART_STOPBIT_2   0x08 //0b00001000

#define UART_DATABIT_5   0x00 //0b00000000
#define UART_DATABIT_6   0x02 //0b00000010
#define UART_DATABIT_7   0x04 //0b00000100
#define UART_DATABIT_8   0x06 //0b00000110

#define MODE_5N1 ((UART_PARITY_NONE)|(UART_STOPBIT_1)|(UART_DATABIT_5))
#define MODE_5E1 ((UART_PARITY_EVEN)|(UART_STOPBIT_1)|(UART_DATABIT_5))
#define MODE_5O1 ((UART_PARITY_ODD) |(UART_STOPBIT_1)|(UART_DATABIT_5))
#define MODE_5N2 ((UART_PARITY_NONE)|(UART_STOPBIT_2)|(UART_DATABIT_5))
#define MODE_5E2 ((UART_PARITY_EVEN)|(UART_STOPBIT_2)|(UART_DATABIT_5))
#define MODE_5O2 ((UART_PARITY_ODD) |(UART_STOPBIT_2)|(UART_DATABIT_5))


#define MODE_6N1 ((UART_PARITY_NONE)|(UART_STOPBIT_1)|(UART_DATABIT_6))
#define MODE_6E1 ((UART_PARITY_EVEN)|(UART_STOPBIT_1)|(UART_DATABIT_6))
#define MODE_6O1 ((UART_PARITY_ODD) |(UART_STOPBIT_1)|(UART_DATABIT_6))
#define MODE_6N2 ((UART_PARITY_NONE)|(UART_STOPBIT_2)|(UART_DATABIT_6))
#define MODE_6E2 ((UART_PARITY_EVEN)|(UART_STOPBIT_2)|(UART_DATABIT_6))
#define MODE_6O2 ((UART_PARITY_ODD) |(UART_STOPBIT_2)|(UART_DATABIT_6))


#define MODE_7N1 ((UART_PARITY_NONE)|(UART_STOPBIT_1)|(UART_DATABIT_7))
#define MODE_7E1 ((UART_PARITY_EVEN)|(UART_STOPBIT_1)|(UART_DATABIT_7))
#define MODE_7O1 ((UART_PARITY_ODD) |(UART_STOPBIT_1)|(UART_DATABIT_7))
#define MODE_7N2 ((UART_PARITY_NONE)|(UART_STOPBIT_2)|(UART_DATABIT_7))
#define MODE_7E2 ((UART_PARITY_EVEN)|(UART_STOPBIT_2)|(UART_DATABIT_7))
#define MODE_7O2 ((UART_PARITY_ODD) |(UART_STOPBIT_2)|(UART_DATABIT_7))


#define MODE_8N1 ((UART_PARITY_NONE)|(UART_STOPBIT_1)|(UART_DATABIT_8))
#define MODE_8E1 ((UART_PARITY_EVEN)|(UART_STOPBIT_1)|(UART_DATABIT_8))
#define MODE_8O1 ((UART_PARITY_ODD) |(UART_STOPBIT_1)|(UART_DATABIT_8))
#define MODE_8N2 ((UART_PARITY_NONE)|(UART_STOPBIT_2)|(UART_DATABIT_8))
#define MODE_8E2 ((UART_PARITY_EVEN)|(UART_STOPBIT_2)|(UART_DATABIT_8))
#define MODE_8O2 ((UART_PARITY_ODD) |(UART_STOPBIT_2)|(UART_DATABIT_8))



/**
 * \brief Buffer for the non blocking UART1
 */
volatile struct RB_t uart1_buffer;


void configure_uart_0(uint8_t mode, uint32_t baud, void (*receivefunction)(uint8_t));

void send_to_bus1(void);

void send_byte_0(uint8_t byte);

void send_byte_bin_0(uint8_t byte);

void send_byte_hex_0(uint8_t byte);

void send_string_0(char* str);

void configure_uart_1(uint8_t mode, uint32_t baud, void (*receivefunction)(uint8_t));

void send_byte_1(uint8_t byte);

void send_byte_bin_1(uint8_t byte);

void send_byte_dec_1(uint8_t byte);

void send_byte_dec16_1(uint16_t word);

void send_byte_hex_1(uint8_t byte);

void send_string_1(char* str);

void send_string_P_1(const char* str);

void disable_rx_interrupt_uart_0(void);

void enable_hwuart_0(void);

void enable_rx_interrupt_uart_0(void);

#endif
