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
	DDRD = 0xFF;
	PORTD = 0xFE;

	//Luefter aus
	SET_BIT(DDRB, PB7);
	CLEAR_BIT(PORTB, PB7);

    //pwm_init();
}


/*======================================================================*/
/* M A I N                                                              */
/*======================================================================*/
int main(void){
	//uint8_t temp[2]={111, 0};
	InitializeMemory();





    //sei();
    node1_init();
    
    while(1);
    return 0;
}
