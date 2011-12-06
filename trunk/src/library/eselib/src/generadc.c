/*! \file generadc.c */

/*======================================================================*/
/*     Project: Treiber generadc.c                                           */
/*    Revision: 1.0                                                     */
/*        Date: 13.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#include "generadc.h"

/*======================================================================*/
/* G L O B A L  V A R I A B L E S                                       */
/*======================================================================*/

typedef void (*functionPointer)(uint16_t a);
volatile functionPointer functiontemp;

volatile static uint8_t prescalearry[15];
volatile static functionPointer pointerarray[15]; 
volatile uint8_t atmchannel=0;

/*======================================================================*/
/* I N T E R R U P T S                                                  */
/*======================================================================*/

//!ADC Interrupt
/*! 
	\pre set_adc_interruptenable(); completed	
*/
SIGNAL(SIG_ADC){
	//take the converted value and start the given methode
	uint16_t temp=ADC;
	functiontemp(temp);
}

/*======================================================================*/
/* P R O C E D U R E S                                                  */
/*======================================================================*/
//!registers a prescaler and a function to a ADC channel
/*! 
	\pre none
	\param channel is one of the ADC input channels 
	\param prescaler is the prescaler and gets asigned to a channel
	\param conversion_function is pointer on a function which gets asigned to the channel
	
*/
void register_channel (uint8_t channel, uint8_t prescale,void (*conversion_function)(uint16_t)){
	//asignment of channel and prescaler or function pointer with an array
	prescalearry[channel]=prescale;
	pointerarray[channel]=conversion_function;
}

//!starts a conversion of the given channel with the given mode
/*! 
	\pre register_channel (uint8_t channel, uint8_t prescale,void (*conversion_function)(uint16_t));
	\param channel choses the channel of the ADC which should be converted 
	\param mode choses between SingleShot and FreeRunning AD- Conversion
	\return is the value of the ADMUX
*/
uint8_t start_conversion(uint8_t channel, uint8_t mode){
	//used the adc.c methodes to configure the ADC
	set_adc_mode(channel);
	set_adc_prescaler(prescalearry[channel]);
	//gets the asigned function pointer of the channel for the interrupt
	functiontemp=pointerarray[channel];
	set_adc_interruptenable();
	//selects between the to possible modes
	if(mode==FREE){ set_freerunning();}
	else {
		ADCSRA |= (1<<ADEN);
		if( (ADCSRA & (1<<ADSC)) == 0){
			ADCSRA |= (1<<ADSC);
		}
	}
	return ADMUX;
}
//!stops the ADC
/*! 
	\pre none
*/
void stop_conversion(void){
	ADCSRA &= ~(1<<ADEN);
}
/*======================================================================*/
/* Main                                                                 */
/*======================================================================*/
/*int main(void){

  return 0;
}*/
