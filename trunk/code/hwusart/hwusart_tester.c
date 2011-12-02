#include <avr/io.h>
#include <stdio.h>

#include "hwusart.h"

void receivedData(uint8_t data);
//void transmittedData(uint8_t data);
//void corruptedData(uint8_t dataSent, uint8_t dataReceived);

int main (void) {

  DDRB = 0xFF;
  PORTB = 0x0F;

  // initialize the usart
  HWUSART_initialize (MODE_8N1,
                      9600,
                      receivedData,
		      NULL,
                      NULL);

  sei();

  //send_byte('a');

  while(1) {
  }

  return 0;
} // end of main

void receivedData(uint8_t data) {
   PORTB = data;

   HWUSART_writeByte(data);
} // end of receive_data

