/*! \file timer3.c */

/*======================================================================*/
/*     Project: Treiber Timer3.c                                        */
/*    Revision: 1.0                                                     */
/*        Date: 14.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*              Perner Martin                                           */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#include "timer3.h"

/*======================================================================*/
/* G L O B A L  V A R I A B L E S                                       */
/*======================================================================*/

/*======================================================================*/
/* I N T E R R U P T S                                                  */
/*======================================================================*/

/*======================================================================*/
/* P R O C E D U R E S                                                  */
/*======================================================================*/

//! setting prescaler of timer3
/*!
	\param prescaler selects the prescaler(0,1,8,64,256,1024) for the timer3
*/
void set_timer3_prescaler(uint16_t prescaler){
	switch(prescaler){
		// only able to select intern modi
		case 0 : TCCR3B &= ~( (1<<CS32)|(1<<CS31)|(1<<CS30) );  
			   break;
		case 1 : TCCR3B &= ~( (1<<CS32)|(1<<CS31) );
			   TCCR3B |= (1<<CS30);
			   break;
		case 8 : TCCR3B &= ~( (1<<CS32)|(1<<CS30) );
			   TCCR3B |= (1<<CS31);
			   break;
		case 64 : TCCR3B &= ~( (1<<CS32) );
			  TCCR3B |= (1<<CS31)|(1<<CS30);
			    break;
		case 256 : TCCR3B &= ~( (1<<CS31)|(1<<CS30) );
			   TCCR3B |= (1<<CS32);
			     break;
		case 1024 : TCCR3B &= ~( (1<<CS31) );
			    TCCR3B |= (1<<CS32)|(1<<CS30);
			      break;
		default: break;
	}
}

//! selecting the mode of operation of timer3
/*!
	\param mode selects the mode for the timer3 (0-3)
*/
void set_timer3_mode(uint8_t mode){
	int modearray[4] = {0,0,0,0};

	modearray[3]= ((mode & 0x08)>>3);
	modearray[2]= ((mode & 0x04)>>2);
	modearray[1]= ((mode & 0x02)>>1);
	modearray[0]= ((mode & 0x01));

	//the register is set dependet on the array bits
	if(modearray[3]) SET_BIT(TCCR3B, WGM33);
		else CLEAR_BIT(TCCR3B, WGM33);
	if(modearray[2]) SET_BIT(TCCR3B, WGM32);
		else CLEAR_BIT(TCCR3B, WGM32);
	if(modearray[1]) SET_BIT(TCCR3A, WGM31);
		else CLEAR_BIT(TCCR3A, WGM31);
	if(modearray[0]) SET_BIT(TCCR3A, WGM30);
		else CLEAR_BIT(TCCR3A, WGM30);
}

//! setting the mode of operation of OC3n Pins
/*!
	\param attribute selects the Pin (A,B,C)
	\param key selects the output behaviour for the timer3 (0-3), 2 = Low - 3 = High
*/
void set_timer3_output(char attribute, uint8_t key){
	int keyarray[2]={0,0};
	
	keyarray[1]= ((key & 0x02)>>1);
	keyarray[0]= ((key & 0x01));
	
	//the register is set dependet on the array bits
	switch(attribute){
		case 'A' :

			if(keyarray[1]) SET_BIT(TCCR3A, COM3A1);
				else CLEAR_BIT(TCCR3A, COM3A1);
			if(keyarray[0]) SET_BIT(TCCR3A, COM3A0);
				else CLEAR_BIT(TCCR3A, COM3A0);

		break;

		case 'B' :
			if(keyarray[1]) SET_BIT(TCCR3A, COM3B1);
				else CLEAR_BIT(TCCR3A, COM3B1);
			if(keyarray[0]) SET_BIT(TCCR3A, COM3B0);
				else CLEAR_BIT(TCCR3A, COM3B0);
		break;

		case 'C' :
			if(keyarray[1]) SET_BIT(TCCR3A, COM3C1);
				else CLEAR_BIT(TCCR3A, COM3C1);
			if(keyarray[0]) SET_BIT(TCCR3A, COM3C0);
				else CLEAR_BIT(TCCR3A, COM3C0);
		break;

		default: break;
	}
}

//! setting OCR3A to given values
/*!
	\param value sets OCR3A 
*/
void set_timer3_OCR3A(uint16_t value){
	OCR3A = value;
}

//! sets OCR3A to TCNT + val
/*!
	\param val Added to TCNT to build OCR3A
*/
void update_timer3_OCRA(uint16_t val)
{
	OCR3A = TCNT3 + val;
}

//! sets OCRC to TCNT + val
/*!
	\param val Added to TCNT to build OCR3C
*/
void update_timer3_OCRC(uint16_t val)
{
	OCR3C = TCNT3 + val;
}

//! setting ICR3 to given value
/*!
	\param value sets ICR3 
*/
void set_timer3_ICR3(uint16_t value){
	ICR3 = value;
}

//! enableing compare A match interrupt
void ena_timer3_OCRA_Int()
{
	ETIMSK |= (1<<OCIE3A);
}

//! disableing compare A match interrupt
void disen_timer3_OCRA_Int()
{
	ETIMSK &= ~(1<<OCIE3A);
}

//! activating compare B match interrupt
void set_timer3_CompareBMatchInterrupt()
{
	ETIMSK |= (1<<OCIE3B);
}

//! enableing compare C match interrupt
void ena_timer3_OCRC_Int()
{
	ETIMSK |= (1<<OCIE3C);
}

//! disableing compare C match interrupt
void disen_timer3_OCRC_Int()
{
	ETIMSK &= ~(1<<OCIE3C);
}

//! activating input capture interrupt
void ena_timer3_IC_Int() {
	ETIMSK |= (1<<TICIE3);
}

//! disableing input capture interrupt
void disen_timer3_IC_Int()
{
	ETIMSK &= ~(1<<TICIE3);
}

//! activation of overflow interrupt
void set_timer3_OverflowInterrupt(){
	ETIMSK |= (1<<TOIE3);
}
