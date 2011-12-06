/**
 * LIB: NODE2
 * 
 * This library contains macros useful for working on node2.
 * 
 * MatNr: 0326077
 * Date: 30.10.2006
 * @author Bernd Froemel <bf@catechsoftworks.biz>
 **/
#ifndef __NODE2_H__
#define __NODE2_H__

/* mostly copy paste from example by Alexander Koessler */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <global.h>

#include "ourMath.h"
#include "timer.h"

#define NODE_ID							(2)

#define PERIODE		100

/*======================================================================*/
/* u64 d i v u32 u32                                                    */
/*----------------------------------------------------------------------*/
/* to get quotient and remainder in one step we need a typedef and      */
/* a function prototyping (external).                                   */
/* the usage of uint64_t for the return value saves some                */
/* cpu instructions and leads to a smaller and faster code.             */
/*======================================================================*/

extern uint64_t u64divu32u32(uint32_t num, uint32_t denom)
     __asm__("__udivmodsi4") __attribute__((__const__));


/*======================================================================*/
/* D I G I T  V A L U E S                                               */
/*======================================================================*/
#define SEGMENT_VAL_OFF					(~0x00)
#define SEGMENT_VAL_DOT					(~0x80)
#define SEGMENT_VAL_0					(~0x3F)
#define SEGMENT_VAL_1					(~0x06)
#define SEGMENT_VAL_2					(~0x5B)
#define SEGMENT_VAL_3					(~0x4F)
#define SEGMENT_VAL_4					(~0x66)
#define SEGMENT_VAL_5					(~0x6D)
#define SEGMENT_VAL_6					(~0x7D)
#define SEGMENT_VAL_7					(~0x07)
#define SEGMENT_VAL_8					(~0x7F)
#define SEGMENT_VAL_9					(~0x6F)
#define SEGMENT_VAL_A					(~0x77)
#define SEGMENT_VAL_B					(~0x7C)
#define SEGMENT_VAL_C					(~0x39)
#define SEGMENT_VAL_D					(~0x5E)
#define SEGMENT_VAL_E					(~0x79)
#define SEGMENT_VAL_F					(~0x71)


/*======================================================================*/
/* D I S P L A Y  P O R T S                                             */
/*======================================================================*/
#define DISPLAY_POSITION_DDR				DDRB
#define DISPLAY_POSITION_OPORT				PORTB
#define DISPLAY_POSITION_IPORT				PINB
#define DISPLAY_DATA_DDR				DDRD
#define DISPLAY_DATA_OPORT				PORTD
#define DISPLAY_DATA_IPORT				PIND
#define DISPLAY_DIGIT_SELECT_PINS			3


/*======================================================================*/
/* M A C R O S                                                          */
/*======================================================================*/
#define DisplayOut(pos, val)                                            \
  DISPLAY_DATA_OPORT = SEGMENT_VAL_OFF;                                 \
  DISPLAY_POSITION_OPORT = (uint8_t)                                    \
    (DISPLAY_POSITION_OPORT & (_BV(8) - _BV(DISPLAY_DIGIT_SELECT_PINS)))\
    | ((~pos) & (_BV(DISPLAY_DIGIT_SELECT_PINS) - 1));                  \
  DISPLAY_DATA_OPORT = val;



#endif
