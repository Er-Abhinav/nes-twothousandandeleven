/*! \file adc.c */

/*======================================================================*/
/*     Project: Treiber adc.c                                           */
/*    Revision: 1.0                                                     */
/*        Date: 13.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#include <adc.h>
/*======================================================================*/
/* G L O B A L  V A R I A B L E S                                       */
/*======================================================================*/

/*======================================================================*/
/* I N T E R R U P T S                                                  */
/*======================================================================*/


/*======================================================================*/
/* P R O C E D U R E S                                                  */
/*======================================================================*/
//!initialiseing the ADC
/*! 
	\pre none	
*/
void init_adc(void){
	DDRF &= ~( (1<<PF0)|(1<<PF1)|(1<<PF2) );
	PORTF &= ~( (1<<PF0)|(1<<PF1)|(1<<PF2) );
}

//!set the adc prescaler
/*! 
	\pre init_adc(); completed
	\param key is the wanted prescaler coded (0-7)	
*/
void set_adc_prescaler(uint8_t key){
	int keyarray[3] = {0,0,0};

	//decode the key parameter
	keyarray[2]= ((key & 0x04)>>2);
	keyarray[1]= ((key & 0x02)>>1);
	keyarray[0]= ((key & 0x01));

	//the register is set dependet on the array bits
	if(keyarray[2]) SET_BIT(ADCSRA, ADPS2);
		else CLEAR_BIT(ADCSRA, ADPS2);
	if(keyarray[1]) SET_BIT(ADCSRA, ADPS1);
		else CLEAR_BIT(ADCSRA, ADPS1);
	if(keyarray[0]) SET_BIT(ADCSRA, ADPS0);
		else CLEAR_BIT(ADCSRA, ADPS0);
}

//!setting the ADC channel
/*! 
	\pre init_adc(); completed
	\param mode select the channel (0-14)	
*/
void set_adc_mode(uint8_t mode){
	int modearray[5] = {0,0,0,0,0};

	//decode the mode parameter
	modearray[4]= ((mode & 0x10)>>4);
	modearray[3]= ((mode & 0x08)>>3);
	modearray[2]= ((mode & 0x04)>>2);
	modearray[1]= ((mode & 0x02)>>1);
	modearray[0]= ((mode & 0x01));

	//the register is set dependent on the array bits
	if(modearray[4]) SET_BIT(ADMUX, MUX4);
		else CLEAR_BIT(ADMUX, MUX4); 
	if(modearray[3]) SET_BIT(ADMUX, MUX3);
		else CLEAR_BIT(ADMUX, MUX3);
	if(modearray[2]) SET_BIT(ADMUX, MUX2);
		else CLEAR_BIT(ADMUX, MUX2);
	if(modearray[1]) SET_BIT(ADMUX, MUX1);
		else CLEAR_BIT(ADMUX, MUX1);
	if(modearray[0]) SET_BIT(ADMUX, MUX0);
		else CLEAR_BIT(ADMUX, MUX0);
}

//!setting the ref voltage of the ADC
/*! 
	\pre init_adc(); completed
	\param value choses between 0 and 3 (on our board only 0 is possible)	
*/
void set_adc_ref(uint8_t value){
	int array[2]={0,0};
	
	//decode the value parameter
	array[1]= ((value & 0x02)>>1);
	array[0]= ((value & 0x01));

	//the register is set dependent on the array bits
	if(array[1]) SET_BIT(ADMUX, REFS1);
		else CLEAR_BIT(ADMUX, REFS1);
	if(array[0]) SET_BIT(ADMUX, REFS0);
		else CLEAR_BIT(ADMUX, REFS0);
}

//!setting the ADC in Free Running Mode
/*! 
	\pre init_adc(); completed
*/
void set_freerunning(void){
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADFR);
	ADCSRA |= (1<<ADSC);
}

//!enable the ADC Interrupt
/*! 
	\pre init_adc(); completed
*/
void set_adc_interruptenable(void){
	ADCSRA |=(1<<ADIE);
}

/*======================================================================*/
/* Main                                                                 */
/*======================================================================*/
/*int main(void){

  return 0;
}*/
