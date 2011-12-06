/*! \file button.h */

//documentation is located in the *.c files cause doxygen errors when the comments are in the *.h files

/*======================================================================*/
/*     Project: Treiber button.h                                        */
/*    Revision: 1.0                                                     */
/*        Date: 13.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "global.h"
#include "timer0.h"
#include "timer.h"

#if defined (__AVR_ATmega128__)

void init_pushbutton(void (*callback)(void));

void measure_intervall(void);

uint16_t read_buttonpresses(void);


#else
#  error "Wrong Target!\n"
#endif

#endif  /* ifndef __BUTTON_H__ */
