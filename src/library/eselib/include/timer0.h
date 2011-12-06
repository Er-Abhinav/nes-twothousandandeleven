/*! \file timer0.h */

//documentation us located in the *.c files cause doxygen errors when the comments are in the *.h files

/*======================================================================*/
/*     Project: Treiber timer0.h                                        */
/*    Revision: 1.0                                                     */
/*        Date: 14.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#ifndef __TIMER02_H__
#define __TIMER02_H__

#include "global.h"

#if defined (__AVR_ATmega128__)

void set_timer0_prescaler(uint16_t prescaler);

void set_timer0_mode(uint8_t mode);

void set_timer0_output(uint8_t key);

void set_timer0_OCR0(uint8_t value);

void set_timer0_CompareMatchInterrupt(void);

void set_timer0_OverflowInterrupt(void);

#else
#  error "Wrong Target!\n"
#endif

#endif  /* ifndef __TIMER02_H__ */
