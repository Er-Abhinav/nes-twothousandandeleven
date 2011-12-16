#include <avr/io.h>
#include <stdio.h>

#include "swusart.h"
#include "singlewire/single_wire_UART.h"

void startBitDetected();
void transmittedData(uint8_t data);
void corruptedData(uint8_t dataSent, uint8_t dataReceived);

int8_t cntr = 0;

int main (void) {
  int i,a;

  DDRC = 0xFF;
  PORTC = 0x10;

  // initialize the usart
  SWUSART_initialize (MODE_8N1,
                      9600,
                      startBitDetected,
		              transmittedData,
                      corruptedData);

  //Set up interrupts
  INITIALIZE_UART_EXTERNAL_INTERRUPT2();
  CLEAR_UART_EXTERNAL_INTERRUPT2_FLAG();
  ENABLE_UART_EXTERNAL_INTERRUPT2();

  sei();

  while(1) {

     //SWUSART_writeByte('a');

	  SWUSART_writeByte('a');

	  for(i=0; i<1000;i++)
		  a++;
  }

  return 0;
} // end of main

void startBitDetected() {
    cntr++; 
    PORTC = cntr;// 0xF0;

    DISABLE_UART_EXTERNAL_INTERRUPT2();
} // end of startBitDetected

void transmittedData(uint8_t data) {
}

void corruptedData(uint8_t dataSent, uint8_t dataReceived) {
}
