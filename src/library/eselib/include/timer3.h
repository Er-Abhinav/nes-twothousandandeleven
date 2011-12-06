/*! \file timer3.h */

//documentation us located in the *.c files cause doxygen errors when the comments are in the *.h files

/*======================================================================*/
/*     Project: Treiber timer3.h                                         */
/*    Revision: 1.0                                                     */
/*        Date: 14.10.2009                                              */
/*      Author: Schwarz Manfred                                         */
/*   Embedded Systems Engineering                                       */
/*======================================================================*/
#ifndef __TIMER3_H__
#define __TIMER3_H__

#include "global.h"

#if defined (__AVR_ATmega128__)

void set_timer3_prescaler(uint16_t prescaler);

void set_timer3_mode(uint8_t mode);

void set_timer3_output(char attribute, uint8_t key);

void set_timer3_ICR3(uint16_t value);

void set_timer3_OCR3A(uint16_t value);

void update_timer3_OCRA(uint16_t value);

void update_timer3_OCRC(uint16_t value);

void ena_timer3_OCRA_Int(void);

void disen_timer3_OCRA_Int(void);

void set_timer3_CompareBMatchInterrupt(void);

void ena_timer3_OCRC_Int(void);

void disen_timer3_OCRC_Int(void);

void ena_timer3_IC_Int(void);

void disen_timer3_IC_Int(void);

void set_timer3_OverflowInterrupt(void);

#else
#  error "Wrong Target!\n"
#endif

#endif  /* ifndef __TIMER3_H__ */
