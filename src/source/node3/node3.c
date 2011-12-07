/*! \file node3.c */

/*======================================================================*/
/*     Project: Treiber node3.c                                           */
/*    Revision: 1.0                                                     */
/*        Date: 13.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#include "node3.h"
//debug
//#include <bargraph.h>
//#define DEBUG
/*======================================================================*/
/* G L O B A L  V A R I A B L E S                                       */
/*======================================================================*/


/*======================================================================*/
/* I N T E R R U P T S                                                  */
/*======================================================================*/

/*======================================================================*/
/* P R O C E D U R E S                                                  */
/*======================================================================*/


void printOut(void) {
#ifdef DEBUG
	StdOutInit();
	SendString("\r\n\r\nInitialization complete!\n\r");
#endif	
}

int main(void){
	InitializeMemory();
	/* disable this anoying whirl sound */
	VENT_OFF();
	LED_DDR |= (1<<LED_RED);
	LED_DDR |= (1<<LED_GREEN);
	DDRD = 0xFF;	
	PORTD = 0xFF;
	//inits of bus_uart, timers
	sei();
	set_adc_ref(0);	
	init_adc();
	//register_channel (TEMP, PRE128,temprature);
        //register_channel (HELLIGKEIT, PRE128,brightness);
	configure_timer(TIMER2, TIMER_PERIODIC, 100, printOut);

	


	while(1);
	return 0;
}
