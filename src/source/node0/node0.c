/*! \file node0.c */

/*======================================================================*/
/*     Project: Treiber node0.c                                           */
/*    Revision: 1.0                                                     */
/*        Date: 13.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/

#include "node0.h"

/*======================================================================*/
/* G L O B A L  V A R I A B L E S                                       */
/*======================================================================*/
//! counter variable 
volatile uint8_t hour=0,
//! counter variable 
min=0,
//! counter variable 
sec=0,
//! counter variable 
tenth=0;
//! array variable to send the time
volatile uint8_t timearray[4]={0,0,0,0};
/*======================================================================*/
/* I N T E R R U P T S                                                  */
/*======================================================================*/

/*======================================================================*/
/* P R O C E D U R E S                                                  */
/*======================================================================*/

//!methode for increasing the time
/*! 
	\pre none	
*/
void bus_time(void){
  //time evaluation beginning from a thenth of a second to hours (max 1 day)
  tenth++;
  //after 1 day we reset all counters to zero
  if(tenth==10){ tenth=0; sec++; }
  if(sec==60){ sec=0; min++; }
  if(min==60){ min=0; hour++; }
  if(hour==24){ hour=0; }
  timearray[0]=tenth;
  timearray[1]=sec;
  timearray[2]=min;
  timearray[3]=hour;
  
  TOGGLE_BIT(LED_PORT_REG, LED_RED);
}

//!methode for sending the time
/*! 
	\pre none	
*/
void sending(void){
  uint8_t tmp[4];
  tmp[0] = timearray[0];
  tmp[1] = timearray[1];
  tmp[2] = timearray[2];
  tmp[3] = timearray[3];
  //send_msg(ADR_ND0_TIME, 4, tmp);
  //green leds indicate the sending of data
  TOGGLE_BIT(LED_PORT_REG, LED_GREEN);
  
}

//!methode for receiving data from the pc
/*! 
	\pre configure_uart_1(MODE_8N1, 230400, &rec_pc); completed
	\param value is the value receive from the pc and choses the function which is performed	
*/
void rec_pc(uint8_t value){
	sei();

}

//!methode for resetting the clock
/*! 
	\pre none	
*/
void reset(void){
	tenth=0;
	sec=0;
	min=0;
	hour=0;
}

void receivedData(uint8_t data) {
   //PORTB = data;

  // HWUSART_writeByte(data);
} // end of receive_data

/*======================================================================*/
/* Main                                                                 */
/*======================================================================*/
int main(void){
	//methode writes values to the memory for memory evaluation
	InitializeMemory();
	//init red and green led
	LED_DDR |= (1<<LED_RED);
	LED_DDR |= (1<<LED_GREEN);
	DDRB = 0xFF;
	PORTB = 0x0F;


	//inits of bus_uart, pc_uart, timers and button
	//protocol_init(RECEIVER_ON, receive_msgfoo);
	//every 100ms the clock is increased
	configure_timer(0, TIMER_PERIODIC, 100, bus_time);
	//every 100ms the clock value is send to the bus system
	configure_timer(2, TIMER_PERIODIC, 100, sending);
//	configure_uart_1(MODE_8N1, 115200, &rec_pc); //FOR DEBUGING
//	send_string_P_1(PSTR("\r\nHello World\r\n"));
//	send_byte_dec16_1(0xfefe); //print out as int
	//send_byte_dec_1(temperature%10);


	init_pushbutton(reset);
	


	// initialize the usart
	HWUSART_initialize(MODE_8N1, 9600, receivedData, NULL, NULL);
	//globale interrupt enable
	sei();

int i, j;
	while(1)
		{
		HWUSART_writeByte('a');
		for(i=0;i<10000; i++) asm("nop");

		}
	return 0;

}
