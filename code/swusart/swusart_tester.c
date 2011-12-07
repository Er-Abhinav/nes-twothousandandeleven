#include <avr/io.h>
#include <stdio.h>

#include "swusart.h"

void startBitDetected();
void transmittedData(uint8_t data);
void corruptedData(uint8_t dataSent, uint8_t dataReceived);

int main (void) {

  DDRC = 0xFF;
  PORTC = 0x0F;

  // initialize the usart
  SWUSART_initialize (MODE_8N1,
                      9600,
                      startBitDetected,
		              transmittedData,
                      corruptedData);

  sei();

  //send_byte('a');

  while(1) {
  }

  return 0;
} // end of main

void startBitDetected() {
   PORTC = 0xF0;
} // end of startBitDetected

void transmittedData(uint8_t data) {
}

void corruptedData(uint8_t dataSent, uint8_t dataReceived) {
}
