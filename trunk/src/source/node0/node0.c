/*! \file node0.c */

/*======================================================================*/
/*     Project: Treiber node0.c                                           */
/*    Revision: 1.0                                                     */
/*        Date: 13.10.2009                                              */
/*      Author: Wir                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/

#include "node0.h"


void receivedData(uint8_t data) {
   //PORTB = data;
	PORTA = ~data;
  // HWUSART_writeByte(data);
	return;
} // end of receive_data


void node0_init(void) {
	//init uart1 ports
	SET_BIT(DDRD, PD3);
	SET_BIT(PORTD, PD3);
	CLEAR_BIT(DDRD, PD2);

	//Push button pull ups on
	CLEAR_BIT(DDRE, PE5);
	CLEAR_BIT(DDRE, PE6);
	SET_BIT(PORTE, PE5);
	SET_BIT(PORTE, PE6);

	//8Leds init
	DDRA = 0xFF; //output
	PORTA = 0xF0; //turn off
}


/*======================================================================*/
/* Main                                                                 */
/*======================================================================*/
int main(void){
	uint16_t i;
	uint8_t cnt=0;
	node0_init();

	// initialize the usart
	HWUSART_initialize(MODE_8N1, 9600, receivedData, NULL, NULL);
	//globale interrupt enable
	sei();
	while(1){
		if(!((1<<PE5) & PINE)) {
			HWUSART_writeByte(cnt++);
			for(i=0; i<60000;i++) asm("nop");
		}
	}
	return 0;

}
