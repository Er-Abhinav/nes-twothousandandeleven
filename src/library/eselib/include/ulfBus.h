/*======================================================================*/
/* Project:						*/
/* MCU      Atmel AVR megaX		*/
/**@file	ulfBus.h
   @author 	Robert, Alex, Nick
   @version 0.1
   @date 	2011/12/05			*/
/*======================================================================*/
#ifndef __ULFBUS_H__
#define __ULFBUS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/io.h>

typedef enum {
	NEW,
	RUN,
	FINISHED,
	QUEUED,
	DEAD
} msg_state;

typedef enum {
	ATTACH,
	DETACH
} attach_detach_t;


typedef enum {
	UNKNOWN = 0,
	BUSY,
	CLAIMED,
	FREE
} l1_bus_status;

/**
 * Start/Sets timer
 * Checks the bus
 */
l1_bus_status l1GetBus(uint16_t arbitration_data);
/**
 * @pre Bus has to be CLAIMED
 */
l1_bus_status l1PushSendBus(uint8_t len, uint8_t *payload);

/**
 * Checks if further data are queued
 * pushes data to the HW-UART
 *
 * CHECK if message to send is a time message
 */
void l1CallbackMsgReceived(void);

uint8_t l1HandlePreSendCallback(attach_detach_t action, uint8_t msgType, \
		void* callback,);


typedef struct {
   uint8_t type : 4;
   uint8_t dest_id : 6;
   uint8_t src_id  : 6;
} l2msgArbitration_t;

typedef struct {
   l2msgArbitration_t head;
   uint8_t size : 4;
   uint8_t crc_size: 4;
} l2msgHeader_t;

typedef struct {
	l2msgHeader_t* header;
	uint8_t* data;
	uint8_t* crc;
	msg_state state;
} l2msg_t;

uint8_t l2init(void);
uint8_t l2attachSend(l2msg_t *msg, uint8_t priority);
uint8_t _l2l1AttachMiddleware(l2msg_t *msg, uint8_t priority);


#ifdef __cplusplus
}
#endif
#endif __ULFBUS_H__
