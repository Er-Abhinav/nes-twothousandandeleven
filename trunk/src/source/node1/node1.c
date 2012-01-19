/*======================================================================*/
/**@file	node1.c
   @brief       Node1 Software
   @author 	Nick Mayerhofer						*/
/* Project:	   LCD-Display						*/
/* Application: LCD							*/
/* MCU          Atmel AVR						*/
/* Revision     $Revision: 1.0 $                                        */
/* Date:        $Date: 2009/10/13 $					*/
/*======================================================================*/
#include "node1.h"

/*======================================================================*/
/* C O N S T A N T S  (place const in flash)                            */
/*======================================================================*/



/*======================================================================*/
/* S U B R O U T I N E S                                                */
/*======================================================================*/
void receivedData(uint8_t data) {
	PORTA = ~data;
	return;
} // end of receive_data

void node1_init(void) {
	//Bus 1 pin settings
	SET_BIT(DDRD, PD3);
	SET_BIT(PORTD, PD3);
	CLEAR_BIT(DDRD, PD2);
	//Luefter aus
	SET_BIT(DDRB, PB7);
	CLEAR_BIT(PORTB, PB7);

	//CPU_board LEDs
	LED_DDR |= ((1<<LED_GREEN)|(1<<LED_RED));
	LED_PORT_REG |= ((1<<LED_GREEN)|(1<<LED_RED));
	//8Leds init
	DDRA = 0xFF; //output
	PORTA = 0xF0; //turn off

	//Push button pull ups on
	CLEAR_BIT(DDRE, PE5);
	CLEAR_BIT(DDRE, PE6);
	SET_BIT(PORTE, PE5);
	SET_BIT(PORTE, PE6);
}


void startBitDetected(void) {
	SET_BIT(LED_PORT_REG, LED_GREEN);
}

/*======================================================================*/
/* M A I N                                                              */
/*======================================================================*/
int main(void){
	int i;
	int cnt=0;
	uint8_t deactivate=0;
	node1_init();

	// initialize the usart
	HWUSART_initialize(MODE_8N1, 9600, receivedData, NULL, NULL);
	SWUSART_initialize (MODE_8N1,9600, startBitDetected);
	//globale interrupt enable
	sei();
	while(1){
		if(!((1<<PE5) & PINE)) {
			if(!deactivate) {
				//HWUSART_writeByte(cnt++);
				SWUSART_writeByte(cnt++);
				for(i=0; i<10000;i++) asm("nop");
			}
			deactivate = 1;
		} else {
			deactivate = 0;
		}
	}
    return 0;
}





void pwm_init(void) {
  ICR1=200;
  OCR1B=100; //BGLight 0==Max
  OCR1C=70; //Contrast ICR1==Max
  //Setup Mode
  CLEAR_BIT(TCCR1A, WGM10);
  TCCR1A |= (1<<WGM11);
  TCCR1B |= (1<<WGM13) | (1<<WGM12);
  //Setup Timer
  TCCR1B |= (1<<CS11); //CS11-Only = Prescaler=8
  //Connect Ports = Turn on OUTPUT
  CLEAR_BIT(TCCR1A, COMB0);
  SET_BIT(TCCR1A,COMB1);
  CLEAR_BIT(TCCR1A, COMC0);
  SET_BIT(TCCR1A,COMC1);
}