/*==========================================================================*/
/* Project:     AVR-GCC SRAM Memory Evaluation                              */
/* Title:       Memory Pointer Functions                                    */
/* Purpose:     SRAM memory ressource analysis of Atmel programs            */
/*                                                                          */
/* File:        mem_eval.h                                                  */ 
/* Revision     0.1                                                         */
/* Date:        10/2003                                                     */
/*                                                                          */
/* Description: Provides macros for reading the stackpointer and the end of */
/*              variable data segment                                       */
/* Software:    AVR-GCC                                                     */
/* Hardware:    AVR AT90 and ATmega series                                  */
/*                                                                          */
/* Authors:     Wilfried Elmenreich, Wolfgang Haidinger                     */
/*              TU Vienna, Real Time Group                                  */
/*==========================================================================*/

#ifndef MEMEVAL
 #define MEMEVAL

 #include <avr/io.h>
 
 #ifdef __AVR_AT90S4433__
  #undef SPH
 #endif
 
 #ifdef SPH
  #define GetStackPtr()({uint8_t *__StackPtr;	\
        asm volatile("in\t%A0,__SP_L__\n\t"		\
                     "in\t%B0,__SP_H__"		\
                     : "=r" (__StackPtr)	\
                     : /*"I" (SPL), "I" (SPH)*/	\
		     );				\
        __StackPtr;})
 #else
  #define GetStackPtr() ({uint8_t *__StackPtr;     \
        asm volatile("in\t%A0,__SP_L__\n\t"        \
                     "clr\t%B0"              \
                     : "=r" (__StackPtr)     \
                     : /* "I" (SPL) */ );           \
        __StackPtr;})
 #endif

 #define EndOfUsedMem()({uint8_t *__EndOfUsedMemPtr;	\
        asm volatile("ldi\t%A0,lo8(__bss_end)\n\t"		\
                     "ldi\t%B0,hi8(__bss_end)"		\
                     : "=r" (__EndOfUsedMemPtr)	\
		     );				\
        __EndOfUsedMemPtr;})
#endif


extern void InitializeMemory(void);
extern void AnalyzeMemory(uint8_t **largest_begin,uint16_t *largest_size);

