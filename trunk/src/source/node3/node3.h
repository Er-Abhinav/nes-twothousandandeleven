/**
 * LIB: NODE3
 * 
 * This library contains macros useful for working on node3.
 * 
 * MatNr: 0326077
 * Date: 30.10.2006
 * @author Bernd Froemel <bf@catechsoftworks.biz>
 **/
#ifndef __NODE3_H__
#define __NODE3_H__


#include <timer.h>
#include <avr/io.h>
#include <generadc.h>
#include <global.h>


#define NODE_ID							(3)

#define VENT_PIN						PB5 /* ventilator */
#define EXT_PIN							PB6 /* JP5 */

#define SENSOR_TEMP_273						PE2
#define SENSOR_TEMP						PE3
#define SENSOR_LIGHT						PE4
#define SENSOR_VENT						PE5

#define VENT_OFF()						DDRB = 1<<VENT_PIN;  \
								PORTB &= ~(1<<VENT_PIN)

#define HELLIGKEIT 2
#define TEMP 9
#define PRE128 7
#define SCHWELLWERT 275
#define TRUE 1
#define FALSE 0
	 
#endif
