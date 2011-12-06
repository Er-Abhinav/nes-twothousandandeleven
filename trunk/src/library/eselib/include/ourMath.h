/*======================================================================*/
/* Project:	SPI LCD Display Driverr					*/
/* MCU          Atmel AVR						*/
/**@file	ourMath.h
   @author 	Nick Mayerhofer
   @version 	0.1
   @date 	2009/11/27						*/
/*======================================================================*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <global.h>

/** Calculates the Sqare root of a value
 * @param[in] op	value
 * @return Square root of op
*/
uint16_t fastSqrt(uint16_t op);

/** Returns true if the value is prime
 * @param[in] op	value
 * @return 0 if not prime, 1 if prime
*/
uint8_t isPrime_8(uint16_t op);

/** Calculates the pow of numbers smaller than 0xFFFF
 * @param[in] base 	Base
 * @param[in] exponent	Exponent
 * @return Base^Exponent
*/
uint16_t _myPow(uint16_t base, uint8_t exponent);
