/*! \file button.c */

/*======================================================================*/
/*     Project: Treiber button.c                                        */
/*    Revision: 1.0                                                     */
/*        Date: 13.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#include "button.h"

/*======================================================================*/
/* G L O B A L  V A R I A B L E S                                       */
/*======================================================================*/

//! typedef of a function pointer
typedef void (*functionPointer)(void);
	//! functionpointer buttonfunction for saving the pointer of methode init_pushbutton();
	functionPointer buttonfunction;

//! measured timer intervall for button pressed
uint16_t intervall=0,
//! old timer intervall
intervallalt=0;

/*======================================================================*/
/* I N T E R R U P T S                                                  */
/*======================================================================*/

//! Handler for ext. interrupt 4
/*! 	
	\pre init_pushbotten completed
*/
SIGNAL(SIG_INTERRUPT4){
	//abhaengigkeit zwischen Button und timer !!!!
	buttonfunction();
	/*if((EICRB & (1<<ISC40))==0){
		configure_timer(2, 2, 5,measure_intervall);
		EICRB |= (1<<ISC40);
	}
	else{
		configure_timer(2, 0, 5,NULL);
		intervallalt=intervall;
		intervall=0;
		EICRB &= ~(1<<ISC40);
	}*/	
}

/*======================================================================*/
/* P R O C E D U R E S                                                  */
/*======================================================================*/

//!initialiseing the virtual botton
/*! 
	(Port inits and trigger inits and interrupt 4 enable
	\pre none
	\param callback is function pointer which is used in the interrupt routine	
*/
void init_pushbutton(void (*callback)(void)){
	DDRE &= ~(1<<PE4);
	PORTE |= (1<<PE4);
	buttonfunction=callback;
	EICRB |= (1<<ISC41);
	EICRB &= ~(1<<ISC40);
	EIMSK |= (1<<INT4);
}

//!measruing the time intervall
/*!
	every 5 ms is an interrupt generated which calls this function and increases the variable intervall 
	\pre init_pushbotten completed	
*/
void measure_intervall(){
	intervall=intervall+5;
}

//!returning the time intervall from the last pressing of the button 
/*! 
	\pre init_pushbotten completed	
	\return intervall measures the last button pressing in ms
*/
uint16_t read_buttonpresses(){
	return intervallalt;
}

/*======================================================================*/
/* Main                                                                 */
/*======================================================================*/
/*int main(void){

  return 0;
}*/
