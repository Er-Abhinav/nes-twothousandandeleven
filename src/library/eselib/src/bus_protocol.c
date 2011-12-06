#include "bus_protocol.h"

/** \file bus_protocol.c
 * \brief The driver for the bus protocol
 * \author Martin Perner, 0725782
 * \version 0.2
 * \date 2009/12/01
 *
 * Driver for the CSMA/CA Protocol with arbitration over message id.
 */

/**
 * \brief Handler which will be called if it is not NULL and a message is recieved 
 */
void (*handler)(uint8_t msg_id, uint8_t msg_length, uint8_t *msg_body);

/**
 * \brief Contains the active state of the send message fsm
 */
volatile uint8_t activeTransmit;
/**
 * \brief Message ID which will be sended or which was recieved if the arbitration is lost
 */
volatile uint8_t sndMsgId;
/**
 * \brief Active bit which will be send next
 */
volatile uint8_t sndBit;
/**
 * \brief Bytes which are recieved from the bus
 */
volatile uint8_t msgCount;
/**
 * \brief MessageId which is recieved from the bus
 */
volatile uint8_t msgId;
/**
 * \brief Amount of bytes which should be recieved from the bus
 */
volatile uint8_t msgLength;
/**
 * \brief Bytes which are recieved from the bus and which will be send with a data paket
 */
volatile uint8_t checkSum;


/**
 * \brief Bytes which are recieved from the bus
 */
uint8_t incData[16];


void recv_msg(uint8_t input);

/**
 * \brief Disables the RX-Interrupt of UART0
 * \param receiver_on Should node receive msg's
 * \param receive_msg Func. for receiving msg's
 * Disables the RX-Interrupt of UART0 and sets the SWUART-Ports for output
 */
void protocol_init(uint8_t receiver_on, void (*receive_msg)(uint8_t msg_id, uint8_t msg_length, uint8_t *msg_body))
{
	if(receiver_on == TRUE) {
		handler = receive_msg;
		//configure uart
		configure_uart_0(MODE_8N1, PROTOCOL_BAUD_RATE, &recv_msg);
		msgCount = 0;
	}
	else {
		configure_uart_0(MODE_8N1, PROTOCOL_BAUD_RATE, NULL);
	}
	SWUART_OPORT  |= ((1<<SWUART_RXD) | (1<<SWUART_TXD));
	SWUART_DDR &= ~(1<<SWUART_RXD);
}

/**
 * \brief Runs the memory evaluation and sends it to the given func.
 * \param id ID of the node for the msg_id
 * \param sendmsg If NULL send_msg is used, else the given func.
 *
 */
void send_memory_stats(uint8_t id, void (*sendmsg)(uint8_t msg_id, uint8_t msg_length, uint8_t *msg_body))
{
	uint8_t *largest_begin,*largest_end;
	uint16_t largest_size;
	uint8_t memarray[6];

	AnalyzeMemory(&largest_begin,&largest_size);
	largest_end = largest_begin + largest_size;

	memarray[5]=((uint16_t)largest_begin >> 8);
	memarray[4]=((uint16_t)largest_begin & 0xFF);
	memarray[3]=((uint16_t)largest_end >> 8); 
	memarray[2]=((uint16_t)largest_end & 0xFF);
	memarray[1]=((uint8_t)(largest_size >> 8)&0xFF);
	memarray[0]=((uint8_t)largest_size & 0xFF);
	if(sendmsg == NULL) {
		send_msg((ADR_MEMEVAL_REPLY_OFFSET+id), 6, memarray);
	}
	else {
		sendmsg((ADR_MEMEVAL_REPLY_OFFSET+id), 6, memarray);
	}
}



/**
 * \brief Interrupt for Output Compare Match A
 *
 * Used for checking if we lost the arbitration and recieving the remaining msg_id
 * if we lost.
 */
SIGNAL(SIG_OUTPUT_COMPARE3A)
{
	if(activeTransmit == STATE_SEND_MSGID) {
		//if we read 0 we have lost the arbitration
		if((SWUART_IPORT & (1<<SWUART_RXD)) == 0x00) {
			//set timer, just in case we need the rec. more bits
			update_timer3_OCRA(OCR_WAIT_BIT);
			disen_timer3_OCRC_Int();

			//sndBit is the next bit, so we shift back one
			//-1 gets as the current upto the MSB to 0, the other bits downto lsb to 1
			//is at least 0x02 when processed
			uint8_t mask = (sndBit>>1) - 1 ;
			//we know that the current is 0, the last bits were correct
			sndMsgId = sndMsgId & mask;

			set_timer3_output('C', 0);

			//we check the last bit and lost
			if(sndBit == 0x80) {
				//the next bit is the stopbit
				activeTransmit = STATE_LOST_ARBITRATION_FINISH;
				disen_timer3_OCRA_Int();
			}
			else {
				//interrupt is already setup for RX of msgid from the bus
				activeTransmit = STATE_LOST_ARBITRATION;
			}
		}
		else {
			//we are dominant, disable OCRA-Interrupt
			disen_timer3_OCRA_Int();
		}
	}
	//we have lost the arbitration, now we rec. the remaining msg_id
	if(activeTransmit == STATE_LOST_ARBITRATION) {
		update_timer3_OCRA(OCR_WAIT_BIT);
		//sndMsgId is filled with correct msgid, the other bits to MSB are 0
		if((SWUART_IPORT & (1<<SWUART_RXD)) == 1) {
			sndMsgId |= (1<<sndBit);
		}

		//shift for the next bit
		sndBit <<= 1;
		if(sndBit == 0x80) {
			//the next bit is the stopbit
			activeTransmit = STATE_LOST_ARBITRATION_FINISH;
			disen_timer3_OCRA_Int();
		}
	}
}

/**
 * \brief Interrupt for Output Compare Match C
 *
 * Used for:
 * 1) waiting if the bus is free
 * 2) sending the msg_id
 * 3) sending and waiting for the stop-bit
 */
SIGNAL(SIG_OUTPUT_COMPARE3C)
{
	switch (activeTransmit) {
	case STATE_WAITING_FOR_BUS:
		//the interrupt ocured without being interrupted by the Input Capture
		//so we can start sending
		disen_timer3_IC_Int();
		disen_timer3_OCRC_Int();
		activeTransmit = STATE_BUS_CLEAR;
	break;
	case STATE_SEND_MSGID:
		//Sending the msg_id
		update_timer3_OCRC(OCR_WAIT_BIT);
		update_timer3_OCRA(OCR_WAIT_HALF_BIT);
		//the bit that we changed to was the last bit
		if(sndBit == 0x80) {
			//setup write of stop-bit
			set_timer3_output('C', 3);
			activeTransmit = STATE_SEND_MSGID_SB;
		}
		else {
			if((sndMsgId & sndBit) != 0) {
				//activate Input Capture
				ena_timer3_OCRA_Int();
			}
			//setup output for next bit.
			sndBit <<= 1;
			//change of bus by compare output mode
			//change with set_timer3_output broke it (OBSCURE)
			if((sndMsgId & sndBit) == 0) {
				//Low
				set_timer3_output('C',2);
			}
			else {
				//High
				set_timer3_output('C',3);
			}
		}
	break;
	case STATE_SEND_MSGID_SB:
		//the stopbit is on the bus, wait one bittime!
		update_timer3_OCRC(OCR_WAIT_BIT);
		activeTransmit = STATE_SEND_MSGID_SB_W;
	break;
	case STATE_SEND_MSGID_SB_W:
		//the stopbit was send, so go to data
		disen_timer3_OCRC_Int();
		activeTransmit = STATE_SEND_DATA;
	break;
	}
}

/**
 * \brief Interrupt for Input Capture
 *
 * Used for checking if the bus is free, if this interrupt is called, there was
 * a negative peak on the bus, meaning that a other node is sending
 */
SIGNAL(SIG_INPUT_CAPTURE3)
{
	disen_timer3_OCRC_Int();
	disen_timer3_IC_Int();
	activeTransmit = STATE_BUS_IN_USE;
}

/**
 * \brief Receives the data from the uart
 * \param input Databyte received from uart
 */
void recv_msg(uint8_t input)
{
	//Increment msgCounter
	msgCount++;
	if(msgCount == 1) {
		//msgid
		msgId = input;
		checkSum = msgId;
	}
	else if(msgCount == 2) {
		//length, if greater 16 than we take it as msgid
		if(input > 16) {
			msgCount = 1;
			msgId = input;
			checkSum = msgId;
		}
		else {
			msgCount = 2;
			msgLength = input + 2;
			checkSum ^= input;
		}
	}
	else if(msgCount <= msgLength) {
		//data bytes
		checkSum ^= input;
		incData[msgCount-3] = input;
	}
	else if(msgCount > msgLength) {
		//checksum
		if(input == checkSum) {
			//if checksum is ok, call the user-defined handler-func
			if(handler != NULL) {
				handler(msgId, msgLength-2, incData);
			}
		}
		msgCount = 0;
	}
}


/**
 * \brief Receives the data from the uart
 * \param msg_id If of the message which should be send
 * \param length Amount of data bytes
 * \param msg_body The data which should be send
 * \return ERR_MSG_NOT_SEND if the msg was not send. ERR_MSG_TO_LONG if the length is not valid, ERR_BUS_IN_USE if a transmition is already running, else the amount of retries for the send.
 */
int8_t send_msg(uint8_t msg_id, uint8_t msg_length, uint8_t *msg_body)
{
	// checks if the parameters a vaild and no other transmition on this node is running
	uint8_t retry = 0, i, checksum;
	if(msg_length > 16 || msg_length == 0) {
		return ERR_MSG_TO_LONG;
	}
	cli();
	if(activeTransmit != STATE_IDLE) {
		return ERR_BUS_IN_USE;
	}

	activeTransmit = STATE_WAIT_FOR_BUS;

	sei();
	while(activeTransmit != STATE_IDLE) {
		switch (activeTransmit) {
			case STATE_WAIT_FOR_BUS:
				//wait for the bus to become free
				if((SWUART_IPORT & (1<<SWUART_RXD)) == 0) {
					//bus is low => reset
					activeTransmit = STATE_BUS_IN_USE;
				}
				else {
					//setup OCRC and IC for check if the bus is free
					set_timer3_mode(0);
					//Set Input Capture for negative edge
					CLEAR_BIT(TCCR3B, ICES);
					//setup for startbit
					set_timer3_output('C', 2);
					update_timer3_OCRC(OCR_WAIT_FREE_LINE);
					//Clear interrupt bits
					ETIFR = 0xFF;
					ena_timer3_OCRC_Int();
					ena_timer3_IC_Int();
					activeTransmit = STATE_WAITING_FOR_BUS;
					//enable timer
					set_timer3_prescaler(PRESCALER);
				}
			break;
			case STATE_BUS_CLEAR:
				//bus is clear, OCRC interrupt occured
				//start transmission
				disable_rx_interrupt_uart_0();
				//setup data for sending
				sndMsgId = 0;
				sndMsgId = msg_id;
				sndBit = 0x01;
				update_timer3_OCRC(OCR_WAIT_BIT);
				//setup set of first bit
				if((sndMsgId & sndBit) == 0) {
					//Low
					set_timer3_output('C',2);
				}
				else {
					//High
					set_timer3_output('C',3);
				}
				ena_timer3_OCRC_Int();
				activeTransmit = STATE_SEND_MSGID;
			break;
			case STATE_BUS_IN_USE:
				//bus is in use
				//best would be to wait for finished rec. ?
				//simply alternativ:
				activeTransmit = STATE_WAIT_FOR_BUS;
			break;
			case STATE_LOST_ARBITRATION_FINISH:
				//only enable RX-Interrupt after complete transmit of msgid, 
				//even if lost arbitration, else we could miss the transmition 
				//or get frame errors
				enable_rx_interrupt_uart_0();
				//call the recv func with the msg which overruled us
				recv_msg(sndMsgId);
				//increment the retries
				retry++;
				if(retry > MAX_RETRY) {
					set_timer3_prescaler(0);
					activeTransmit = STATE_IDLE;
					return ERR_MSG_NOT_SEND;
				}
				activeTransmit = STATE_WAIT_FOR_BUS;
			break;

			case STATE_WAITING_FOR_BUS:
			case STATE_SEND_START_BIT:
			case STATE_LOST_ARBITRATION:
			case STATE_SEND_MSGID_SB_W:
			case STATE_SEND_MSGID_SB:
			case STATE_SEND_MSGID:
				//just busy-wait for timer interrupt
				//no sleep here, could cause a stuck in this state
				//e.g. interrupt occurs after going into this section, after the
				//ISR the mcu will go to sleep. so the fsm would be in a wrong state
			break;


			case STATE_SEND_DATA:
				//sending data
				//disable timer3
				set_timer3_prescaler(0);
				//enable HWUART
				enable_hwuart_0();

				checksum = msg_id;
				checksum ^= msg_length;
				send_byte_0(msg_length);

				for(i = 0; i < msg_length; i++) {
					checksum ^= msg_body[i];
					send_byte_0(msg_body[i]);
				}
				send_byte_0(checksum);

				//reenable the uart RX-Interrupt
				enable_rx_interrupt_uart_0();

				//we send a message, if the msgCount was out-of-sync before
				//sending, we can reset him now.
				msgCount = 0;
				activeTransmit = STATE_IDLE;
			break;
		}
	}
	return retry;
}

