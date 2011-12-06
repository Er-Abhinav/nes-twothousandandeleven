/*! \file adc.h */

//documentation us located in the *.c files cause doxygen errors when the comments are in the *.h files

/*======================================================================*/
/*     Project: Treiber adc.h                                           */
/*    Revision: 1.0                                                     */
/*        Date: 13.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#ifndef __ADC_H__
#define __ADC_H__

#include "global.h"

#if defined (__AVR_ATmega128__)

void init_adc(void);

void set_adc_prescaler(uint8_t key);

void set_adc_ref(uint8_t value);

void set_adc_mode(uint8_t mode);

void set_freerunning(void);

void set_adc_interruptenable(void);

#else
#  error "Wrong Target!\n"
#endif

#endif  /* ifndef __BARGRAPH_H__ */
