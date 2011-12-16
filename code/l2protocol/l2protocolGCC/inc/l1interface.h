#ifndef L1INTERFACE
#define L1INTERFACE

#include <stdio.h>
#include <stdint.h>

typedef struct l1Msg
{
	uint8_t  *payload;
	uint8_t   payloadLen;
} l1Msg_t;

// methods
void HWUSART_initialize(uint8_t mode,
                        uint32_t baud,
                        void(*byteReceivedCallback)(uint8_t data),
                        void(*byteTransmittedCallback)(uint8_t data),
                        void(*byteCorruptedCallback)(uint8_t dataSent,
                                                     uint8_t dataReceived));

void HWUSART_enable();
void HWUSART_disable();

void HWUSART_writeByte (uint8_t data);

void SWUSART_initialize(uint8_t mode,
                        uint32_t baud,
                        void(*startBitDetectedCallback)(),
                        void(*byteTransmittedCallback)(uint8_t data),
                        void(*byteCorruptedCallback)(uint8_t dataSent,
                                                     uint8_t dataReceived));

void SWUSART_writeByte (uint8_t data);

void SWUSART_enable();
void SWUSART_disable();

#endif
