/*! \file generadc.h */

//documentation us located in the *.c files cause doxygen errors when the comments are in the *.h files

/*======================================================================*/
/*     Project: Treiber generadc.h                                           */
/*    Revision: 1.0                                                     */
/*        Date: 13.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#ifndef __GENERADC_H__
#define __GENERADC_H__

#include "global.h"
#include "adc.h"

#if defined (__AVR_ATmega128__)

void register_channel (uint8_t channel, uint8_t prescale,void (*conversion_function)(uint16_t)) ;

uint8_t start_conversion(uint8_t channel, uint8_t mode);

void stop_conversion(void);

uint16_t get_value(uint8_t channel);

#define FREE   1
#define SINGLE 0

#else
#  error "Wrong Target!\n"
#endif

#endif  /* ifndef __GENERADC_H__ */
