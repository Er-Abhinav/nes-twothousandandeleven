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
/* G L O B A L  V A R I A B L E S  (SRAM)                               */
/*======================================================================*/
volatile uint8_t prot_sendBright=0;		//!< Buffer for received brightness value
volatile uint8_t prot_sendTemp[2]={0,0};	//!< Buffer for received temperature value
volatile uint8_t flagtemp=0;		//!< Marks temperature to send it from main
volatile uint8_t flagbright=0;		//!< Marks brightness to send it from main

/*======================================================================*/
/* S U B R O U T I N E S                                                */
/*======================================================================*/
void ports_init(void) {
	DDRD=0xFF;
	PORTD=0xFE;
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



void node1_init(void) {
    //uint8_t i;
    ports_init();
    //pwm_init();
}



SIGNAL(SIG_INTERRUPT4){
	TOGGLE_BIT(PORTB, PB5);
	//if( (PORTB & (1<<PB5)) == 0){}
}

/*======================================================================*/
/* M A I N                                                              */
/*======================================================================*/
int main(void){
	//uint8_t temp[2]={111, 0};
	InitializeMemory();
	DDRB |= (1<<PB5);
	PORTB &= ~(1<<PB5);

	LED_DDR |= ((1<<LED_GREEN)|(1<<LED_RED));

	EICRB |= (1<<ISC41);
	EICRB &= ~(1<<ISC40);
	EIMSK |= (1<<INT4);
	//init_pushbutton(&bulb);

    //sei();
    node1_init();
    
    while(1);
    return 0;
}
