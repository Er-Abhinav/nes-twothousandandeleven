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

//!receive routine for bus communication
/*! 
	\pre protocol_init(RECEIVER_ON, receive_msgfoo); completed	
	\param msg_id is the id from the received data 
	\param msg_length is the length from the received data
	\param msg_body is the received data
*/
void receive_msgfoo(uint8_t msg_id, uint8_t msg_length, uint8_t *msg_body){
	uint16_t temperature=0,prim=0;
	//the red led indicates the receiving of data
	TOGGLE_BIT(LED_PORT_REG, LED_RED);
	sei();
	//each data gets differntly displayed
	//constants get loaded from the flash memory
	/*switch(msg_id){
		case ADR_ND1_TEMP: 
			temperature=msg_body[0];
			temperature|=(msg_body[1]<<8);
			send_string_P_1(PSTR("TEMPERATUR:"));
			send_byte_dec16_1(temperature/10);
			send_string_P_1(PSTR("."));
			send_byte_dec_1(temperature%10);
			send_string_P_1(PSTR("*C\n\r"));
			break;
		case ADR_ND1_BRIGHTN:
			send_string_P_1(PSTR("HELLIGKEIT:"));
			send_byte_dec_1(msg_body[0]);
			send_string_P_1(PSTR("%\n\r"));
			break;
		case ADR_ND2_ERR_PRIM:
			prim=msg_body[0];
			prim|=(msg_body[1]<<8);
			send_string_P_1(PSTR("PRIMZAHL:"));
			send_byte_dec16_1(prim);
			send_string_P_1(PSTR("\n\r"));
			break;
		case ADR_ND3_ERR_OVERFLOW: 
			send_string_P_1(PSTR("TEMPERATUR zu hoch!"));
			send_string_P_1(PSTR("\n\r"));
			break;
		case ADR_MEMEVAL_REPLY_OFFSET:
		case ADR_MEMEVAL_REPLY_N1:
		case ADR_MEMEVAL_REPLY_N2:
		case ADR_MEMEVAL_REPLY_N3:
			send_string_P_1(PSTR("Node:"));
			//we evaluate the node number from the id 
			//therefor we have only 1 chase for every memory data we get
			send_byte_dec_1(msg_id-ADR_MEMEVAL_REPLY_OFFSET);
			send_string_P_1(PSTR("\n\rSize: "));
			send_byte_dec16_1((msg_body[1]<<8)|msg_body[0]);
			send_string_P_1(PSTR("\n\rBegin: "));
			send_byte_dec16_1((msg_body[3]<<8)|msg_body[2]);
			send_string_P_1(PSTR("\n\rEnd: "));
			send_byte_dec16_1((msg_body[5]<<8)|msg_body[4]);
			send_string_P_1(PSTR("\n\r"));
			break;
		default: break;
	}*/
}

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
	//uint8_t tmp[2];
	//if the value is between 1-3 the memeval from the nodes 1-3 is requested
	//if(value>'0' && value <= '3'){
	//	send_msg((ADR_MEMEVAL_REQ_OFFSET+(value-'0')), 2, tmp);
	//}
	//if value is 0 the memeval from node0 is send to the pc
	//else if(value == '0') {
	//	send_memory_stats(NODE_ID, &receive_msgfoo);
	//}
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

/*======================================================================*/
/* Main                                                                 */
/*======================================================================*/
int main(void){
	//methode writes values to the memory for memory evaluation
	InitializeMemory();
	//init red and green led
	LED_DDR |= (1<<LED_RED);
	LED_DDR |= (1<<LED_GREEN);
	DDRB=0xFF;
	//globale interrupt enable
	sei();
	//inits of bus_uart, pc_uart, timers and button
	//protocol_init(RECEIVER_ON, receive_msgfoo);
	//every 100ms the clock is increased
	configure_timer(0, TIMER_PERIODIC, 100, bus_time);
	//every 100ms the clock value is send to the bus system
	configure_timer(2, TIMER_PERIODIC, 100, sending);
	configure_uart_1(MODE_8N1, 230400, &rec_pc);
	init_pushbutton(reset);
	
	while(1);
	return 0;

}
