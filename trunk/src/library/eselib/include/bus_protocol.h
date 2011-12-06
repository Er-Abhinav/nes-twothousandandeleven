#ifndef __BUS_PROTOCOL_H__
#define __BUS_PROTOCOL_H__

/** \file bus_protocol.h
 * \brief The driver for TIMER0/2
 * \author Martin Perner, 0725782
 * \version 0.9
 * \date 2009/10/29
 *
 * Driver for TIMER0/2
 */

#include <bus_addressing.h>
#include <uart.h>
#include <timer3.h>

/**
 * \brief Enable or Disable receiving of messages
 **/
#define RECEIVER_ON  1
#define RECEIVER_OFF 0

/**
 * \brief Amount of maximum retries before returning ERR_MSG_NOT_SEND
 */
#define MAX_RETRY 10

/**
 * \brief Is return if MAX_RETRY was reached
 */
#define ERR_MSG_NOT_SEND -1
/**
 * \brief Is returned if the length of the message is higher than allowed
 */
#define ERR_MSG_TO_LONG  -2
/**
 * \brief Is returned if send_msg is already called by another function on the node
 */
#define ERR_BUS_IN_USE   -3

/**
 * \brief States of the FSM
 */
#define STATE_IDLE             0
#define STATE_WAIT_FOR_BUS     1
#define STATE_BUS_CLEAR        2
#define STATE_SEND_DATA        3
#define STATE_BUS_IN_USE       4
#define STATE_LOST_ARBITRATION 5	//state is active if arbitration is lost and the rest of the msgid needs to be rx'd
#define STATE_WAITING_FOR_BUS  6
#define STATE_SEND_START_BIT   7
#define STATE_SEND_MSGID       8
#define STATE_SEND_MSGID_SB    9	//SB == StopBit!
#define STATE_SEND_MSGID_SB_W 10	//W == Wait
#define STATE_LOST_ARBITRATION_FINISH 11

/**
 * \brief Baud Rate for the Protocol
 */
#define PROTOCOL_BAUD_RATE    9600ul //works with prescaler 64, 8
//#define PROTOCOL_BAUD_RATE    19200ul //works with prescaler  64
//#define PROTOCOL_BAUD_RATE    38400ul//works with prescaler 8, 64
//#define PROTOCOL_BAUD_RATE    57600ul //64

#if PROTOCOL_BAUD_RATE == 38400ul
	#define PRESCALER		8ul
	#define DIRTY_HARRY	8
#else
	#define PRESCALER		64ul
	#define DIRTY_HARRY	0
#endif

/**
 * \brief Amounts of bits to wait before sending
 */
#define PROTOCOL_WAIT_LINE    20

/**
 * \brief Time of a Bit with the current baud rate
 */
#define OCR_WAIT_BIT    (uint16_t)(F_CPU/(((PROTOCOL_BAUD_RATE) * (PRESCALER))-1))-1-DIRTY_HARRY

/**
 * \brief Time of half a Bit with the current baud rate
 */
#define OCR_WAIT_HALF_BIT (uint16_t)((OCR_WAIT_BIT)/2)

/**
 * \brief Time of the Bits which should be waited before declaring the bus free
 */
#define OCR_WAIT_FREE_LINE (uint16_t)((PROTOCOL_WAIT_LINE * OCR_WAIT_BIT))

void protocol_init(uint8_t receiver_on, void (*receive_msg)(uint8_t msg_id, uint8_t msg_length, uint8_t *msg_body));

int8_t send_msg(uint8_t msg_id, uint8_t msg_length, uint8_t *msg_body);

void receive_msg(uint8_t msg_id, uint8_t msg_length, uint8_t *msg_body);

void send_memory_stats(uint8_t, void (*sendmsg)(uint8_t msg_id, uint8_t msg_length, uint8_t *msg_body));

#endif
