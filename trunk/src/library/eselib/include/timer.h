#ifndef __TIMER_H__
#define __TIMER_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

#include <global.h>

/** \file timer.h
 * \brief Header for the TIMER0/2 driver
 * \author Martin Perner, 0725782
 * \version 0.9
 * \date 2009/10/29
 *
 * Header for the driver for TIMER0/2
 * 
 */

/**
 * \brief Frequency for which TIMER_OCR and TIMER_CORRECTION are configured
 */
#define TIMER_CPU  14745600

#if !defined CPU_CLK || (CPU_CLK != TIMER_CPU)
	#error "Timer-Values are set for a different clock"
#endif

/**
 * \brief Mode for periodic execution of the timer
 */
#define TIMER_PERIODIC 1
/**
 * \brief Stop after one execution
 */
#define TIMER_ONESHOT  2
/**
 * \brief Stops timer operation
 */
#define TIMER_STOP     3


#define TIMER0 0
#define TIMER2 2

/**
 * \brief TOP-Value of the Timer
 * \note Value is set for a Prescaler of 64 and a \f$f_{CLK} = 14745600 Hz\f$
 *
 * The value is calculated by \f$OCR = \frac{f_{CLK}}{(f_{OCR} * PRESCALER)-1}\f$
 * with a \f$f_{OCR}\f$ of one \f$OCR = 229.4 \sim 0xE5\f$ there is a little 
 * (1.736ns) between 1ms and the value that can be provided
 */
#define TIMER_OCR 0xE5

/**
 * \brief Bound for correction of the TOP-Value
 * \note Value is set for a Prescaler of 64 and a \f$f_{CLK} = 14745600 Hz\f$
 *
 * Thus the TOP-Value of the Timer can not be set exact we calculate a value for
 * which we can add an additional timer-overflow for more precision.
 * We get a timer run time (for a \f$ f_{CLK} = 14745600 Hz\f$) of 
 * \f$\frac{1}{f_{OCR}}=\frac{PRESCALER * (1+OCR)}{f_{CLK}} = 0.9982ms.\f$ 
 * Which gives us a variance of 1.736ns. With \f$ \frac{1ms}{1.736ns} \f$
 * we get a value of approximate 576. That means after 576 timer overflow we 
 * can add an addition overflow for correction the variance.
 */
#define TIMER_CORRECTION      576

void configure_timer(uint8_t id, uint8_t mode, uint16_t duration, void (*handler)(void));

#endif
