/************************************************************************/
/** @file	ringBuff.c
   @brief       Implements a ringBuffer
  									*/
/* MCU          Atmel AVR						*/
/* Revision     $Revision: 1.0 $					*/
/* Date:        $Date: 2009/11/01 $					*/
/* Author(s):   Nick Mayerhofer						
 ************************************************************************/

#include "ringBuff.h"
#include <avr/interrupt.h>


void RB_Init(struct RB_t *f) { 
	f->rbRead=	0;
	f->rbWrite=	0;
	f->rbTuples = 	0; //Element Counter
}


Q RB_Read(struct RB_t *f) {
  uint8_t gieSave=(SREG& 0x80);
  cli();
  
  if(f->rbTuples ==0) return 0; //Buffer empty
  if(f->rbRead >= BUFFSIZE)  f->rbRead=BOTTOM;
  f->rbTuples--;
  Q tmp = f->buf[f->rbRead++]; 
  
  SREG|=gieSave;
  return tmp;
}

//Pointer laeuft von BOTTOM to Top
uint8_t RB_Write(struct RB_t *f, Q el) {
  uint8_t gieSave=(SREG& 0x80);
  cli();
  
  //Check if begin/end of Ring is reached
  if(f->rbWrite >= BUFFSIZE)  f->rbWrite=BOTTOM;
  if(f->rbTuples < BUFFSIZE) { //Buffer not Full
	f->rbTuples++;
	f->buf[(f->rbWrite)++]=el;
  } else
	return RB_FULL;

  SREG|=gieSave;
  return RB_OK;
}

uint8_t RB_IsEmpty(struct RB_t *f) {
	if(f->rbTuples ==0)
		return 1;
	return 0;
}

uint16_t RB_FreeSpace(struct RB_t *f) {
	return BUFFSIZE - f->rbTuples;
}
