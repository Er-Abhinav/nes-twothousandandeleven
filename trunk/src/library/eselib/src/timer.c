#include <timer.h>

/** \file timer.c
 * \brief The driver for TIMER0/2
 * \author Martin Perner, 0725782
 * \version 0.9
 * \date 2009/10/29
 *
 * Driver for TIMER0/2
 */

/**
 * \brief Mode of TIMER0
 */
uint8_t TIMER0_MODE;

/**
 * \brief Mode of TIMER2
 */
uint8_t TIMER2_MODE;

/**
 * \brief Duration of TIMER0
 */
volatile uint32_t TIMER0_duration;

/**
 * \brief Current duration of TIMER0
 */
volatile uint32_t TIMER0_act;

/**
 * \brief Duration of TIMER2
 */
volatile uint32_t TIMER2_duration;

/**
 * \brief Current duration of TIMER2
 */
volatile uint32_t TIMER2_act;

/**
 * \brief TIMER0 handler function
 */
void (*timer_0_handler)(void);
/**
 * \brief TIMER2 handler function
 */
void (*timer_2_handler)(void);

/**
 * \brief Corrects the variance of the timer run time
 * \param duration Wanted duration
 * \param id Id of the timer
 *
 * Sets the timer duration to \e duration plus the integer of \e duration 
 * divided by \b TIMER_CORRECTION 
 */
void setDuration(uint16_t duration, uint8_t id);

/**
 * \brief Configures the given timer to the given mode with the given duration
 * \param id Id of the timer which should be configured
 * \param mode Mode can be TIMER_PERIODIC, TIMER_ONESHOT or TIMER_STOP
 * \param duration Run time of the timer in ms
 * \param handler Interrupts handler which should be called after an interrupt
 * \note Only TIMER0 and TIMER2 are supported
 * 
 */
void configure_timer(uint8_t id, uint8_t mode, uint16_t duration, void (*handler)(void))
{
	__atomic_start();
	if(id == TIMER0) {
		if(mode == TIMER_STOP) {
			//disable clock
			TCCR0 &= ~((1<<CS2)|(1<<CS1)|(1<<CS0)); 
			//disable interrupt
			TIMSK &= ~(1<<OCIE0);
		} else {
			//prescaler 64, ctc
			TCCR0 |= ((1<<CS2)|(1<<WGM1));
			TCCR0 &= ~((1<<CS1)|(1<<CS0)|(1<<WGM0));

			TCNT0 = 0x00;
			OCR0 = TIMER_OCR;
			TIMER0_MODE = mode;
			TIMER0_act = 0;

			//set timer run time
			setDuration(duration, id);

			//if interrupt handler is not NULL
			if(handler != NULL) {
				timer_0_handler = handler;
				//enable interrupt
				TIMSK |= (1<<OCIE0);
			}
		}
	}
	else if(id == TIMER2) {
		if(mode == TIMER_STOP) {
			//disable clock
			TCCR2 &= ~((1<<CS2)|(1<<CS1)|(1<<CS0));
			//disable interrupt
			TIMSK &= ~(1<<OCIE2);
		} else {
			//prescaler 64, ctc
			TCCR2 |= ((1<<CS1)|(1<<CS0)|(1<<WGM1));
			TCCR2 &= ~((1<<CS2)|(1<<WGM0));

			TCNT2 = 0x00;
			OCR2 = TIMER_OCR;
			TIMER2_MODE = mode;
			TIMER2_act = 0;

			//set timer run time
			setDuration(duration, id);

			//if interrupt handler is not NULL
			if(handler != NULL) {
				timer_2_handler = handler;
				//enable interrupt
				TIMSK |= (1<<OCIE2);
			}
		}
	}
	__atomic_end();
}

void setDuration(uint16_t duration, uint8_t id)
{
	uint32_t *timer_duration;
	if(id == TIMER0) {
		timer_duration = (uint32_t*)&TIMER0_duration;
	}
	else {
		timer_duration = (uint32_t*)&TIMER2_duration;
	}

	*timer_duration = duration;
	while(duration > TIMER_CORRECTION) {
		duration -= TIMER_CORRECTION;
		(*timer_duration)++;
	}
}

/**
 * \brief Interrupt handler for TIMER0
 */
SIGNAL(SIG_OUTPUT_COMPARE0)
{
	++TIMER0_act;

	//If wanted run time is hit
	if(TIMER0_act >= TIMER0_duration) {
		if(TIMER0_MODE == TIMER_ONESHOT) {
			//disable CLK
			TCCR0 &= ~((1<<CS2)|(1<<CS1)|(1<<CS0));
		}
		else {
			TIMER0_act = 0;
		}
		timer_0_handler();
	}
}

/**
 * \brief Interrupt handler for TIMER0
 */
SIGNAL(SIG_OUTPUT_COMPARE2)
{
	++TIMER2_act;

	if(TIMER2_act >= TIMER2_duration) {
		if(TIMER2_MODE == TIMER_ONESHOT) {
			//disable CLK
			TCCR2 &= ~((1<<CS2)|(1<<CS1)|(1<<CS0));
		}
		else {
			TIMER2_act = 0;
		}
		timer_2_handler();
	}
}
