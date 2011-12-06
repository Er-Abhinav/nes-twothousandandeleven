/*! \file timer0.c */

/*======================================================================*/
/*     Project: Treiber Timer0.c                                      */
/*    Revision: 1.0                                                     */
/*        Date: 14.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#include "timer0.h"

/*======================================================================*/
/* G L O B A L  V A R I A B L E S                                       */
/*======================================================================*/

/*======================================================================*/
/* I N T E R R U P T S                                                  */
/*======================================================================*/

/*======================================================================*/
/* P R O C E D U R E S                                                  */
/*======================================================================*/

//! setting timer0 prescaler (only intern modi)
/*!
	\param prescaler selects the prescaler(0,1,8,32,64,128,256,1024) for the timer0
*/
void set_timer0_prescaler(uint16_t prescaler){
	switch(prescaler){
		// only able to select intern modi
		case 0 : TCCR0 &= ~( (1<<CS02)|(1<<CS01)|(1<<CS00) );  
			   break;
		case 1 : TCCR0 &= ~( (1<<CS02)|(1<<CS01) );
			 TCCR0 |= (1<<CS00);
			   break;
		case 8 : TCCR0 &= ~( (1<<CS02)|(1<<CS00) );
			 TCCR0 |= (1<<CS01);
			   break;
		case 32 : TCCR0 &= ~( (1<<CS02) );
			  TCCR0 |= (1<<CS01)|(1<<CS00);
			    break;
		case 64 : TCCR0 &= ~( (1<<CS01)|(1<<CS00) );
			  TCCR0 |= (1<<CS02);
			     break;
		case 128 : TCCR0 &= ~( (1<<CS01) );
			   TCCR0 |= (1<<CS02)|(1<<CS00);
			      break;
		case 256 : TCCR0 &= ~( (1<<CS00) );
			   TCCR0 |= (1<<CS02)|(1<<CS01);
			      break;
		case 1024 : TCCR0 |= (1<<CS02)|(1<<CS01)|(1<<CS00);
			      break;
		default: break;
	}
}

//! selecting the mode of operation of timer0
/*!
	\param mode selects the mode for the timer0 (0-3)
*/
void set_timer0_mode(uint8_t mode){
	int modearray[2] = {0,0};
	
	modearray[1]= ((mode & 0x02)>>1);
	modearray[0]= ((mode & 0x01));
	
	//the register is set dependet on the array bits 
	if(modearray[1]) SET_BIT(TCCR0, WGM01);
		else CLEAR_BIT(TCCR0, WGM01);
	if(modearray[0]) SET_BIT(TCCR0, WGM00);
		else CLEAR_BIT(TCCR0, WGM00);
	
	
}

//! setting the mode of operation of OC0
/*!
	\param key selects the output behaviour for the timer0 (0-3)
*/
void set_timer0_output(uint8_t key){
	int keyarray[2]={0,0};
	
	keyarray[1]= ((key & 0x02)>>1);
	keyarray[0]= ((key & 0x01));

	//the register is set dependet on the array bits 
	if(keyarray[1]) SET_BIT(TCCR0, COM01);
		else CLEAR_BIT(TCCR0, COM01);
	if(keyarray[0]) SET_BIT(TCCR0, COM00);
		else CLEAR_BIT(TCCR0, COM00);

}

//! OCR0 is set to the given value
/*!
	\param value sets OCR0 
*/
void set_timer0_OCR0(uint8_t value){
	OCR0 = value;
}

//! Compare Match Interrupt is enabled
void set_timer0_CompareMatchInterrupt(){
	TIMSK |= (1<<OCIE0);
}

//! Overflow interrupt is enabled
void set_timer0_OverflowInterrupt(){
	TIMSK |= (1<<TOIE0);
}
