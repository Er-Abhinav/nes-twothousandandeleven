/*======================================================================*/
/**@file	ourMath.c
   @brief       Small math functions
   @author 	Nick Mayerhofer						*/
/* Project:	   LCD-Display						*/
/* Application: LCD							*/
/* MCU          Atmel AVR						*/
/* Revision     $Revision: 1.0 $                                        */
/* Date:        $Date: 2009/10/13 $					*/
/*======================================================================*/
#include "ourMath.h"

uint16_t fastSqrt(uint16_t op) {
    uint16_t res = 0;
    uint16_t one = (1 << 14); // The second-to-top bit is set: 1L<<30 for long
    // "one" starts at the highest power of four <= the argument.
    while (one > op) {
        one >>= 2;
    }
    while (one != 0) {
        if (op >= res + one) {
            op -= res + one;
            res = (res >> 1) + one;
        } else {
            res >>= 1;
	    }
        one >>= 2;
    }
    return res;
}


uint8_t isPrime_8(uint16_t op) {
	//we need 16-bit, sqrt of 65535 is 255, abort of if needs 256
   uint8_t i, top; 

   if(op <= 1) return FALSE; //0,1
   if(op < 4) return TRUE;	//2, 3
   if((op & 0x01) == 0x00) return FALSE; //even numbers (checks if lsb is set)
   if(op < 9) return TRUE; //5,7
   if((op%3) == 0) return FALSE; //multiple of 3

   top = fastSqrt(op);
   //no even number, and no duplicte of 3 needs to be checked.
   //i checks a odd number
   //i+2 checks another odd number
   //i+4 is a multiple of three
   //i+6 is the next odd number that needs to be checked
   for(i=5; i<=top;i=i+6) {
      if((op%i) == 0) {
         return FALSE;
      }
	  if((op%(i+2)) == 0) {
         return FALSE;
      }
	  //next increment would lead to overrun
	  //last check was 253; 254 is even, 255 is a multiple of 5, which was checked first
	  if(i == 251) {
		  break;
	  }
   }
   return TRUE;
}


uint16_t _myPow(uint16_t base, uint8_t exponent) {
	if(exponent==0) return 1;
	for(exponent=exponent; exponent>1; exponent--)
		base*=base;
	return base;
}
