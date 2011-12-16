/*======================================================================*/
/**@file	node2.c
   @brief       Node2 Software
   @author 	Nick Mayerhofer						*/
/* MCU          Atmel AVR						*/
/* Revision     $Revision: 1.0 $                                        */
/* Date:        $Date: 2009/10/27 $					*/
/*======================================================================*/
#include "node2.h"


/*======================================================================*/
/* G L O B A L  V A R I A B L E S  (SRAM)                               */
/*======================================================================*/
volatile uint16_t prime=0;

/*======================================================================*/
/* M A I N                                                              */
/*======================================================================*/
/** Handles the message receiving
*/


void node2Init(void) {
	LED_DDR |= ((1<<LED_GREEN)|(1<<LED_RED));
	LED_PORT_REG |= ((1<<LED_GREEN)|(1<<LED_RED));
}

/** Runs the prime check and gives a Signal to 
 * send out the prime value
*/
void node2DoIt(void) {
	TOGGLE_BIT(LED_PORT_REG, LED_GREEN);
	TOGGLE_BIT(LED_PORT_REG, LED_RED);
}

int main(void) {
   InitializeMemory();
   node2Init();
   
   sei();
   configure_timer(0, TIMER_PERIODIC, PERIODE, &node2DoIt);
   	
   while(1) {
      //Sends out prime value if set from node2DoIt()
      if(prime){
         //tmp[0]=(prime & 0xFF);
         //tmp[1]=(prime>>8);
         prime=0;
         TOGGLE_BIT(LED_PORT_REG, LED_GREEN);
         }	
      }
   return 0;
}
