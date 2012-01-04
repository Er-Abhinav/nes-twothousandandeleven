#include <avr/io.h>              //Device specific register/Bit definitions.
#include <avr/interrupt.h>              //The __enable_interrupt() intrinsic.
#include "stdint.h"     //Integer types.

#ifndef FALSE
  #define FALSE 0
  #define TRUE !FALSE
#endif

// Baud rate settings (WAIT_ONE - PRESCALER):
//  Baud Rate     1MHz      2Mhz      4MHz      8MHz
//     4800     207 - 1    51 - 8   103 - 8   (207 - 8)
//     9600     103 - 1   207 - 1    51 - 8   103 - 8
//    19200       NA      103 - 1   207 - 1    51 - 8
//    28800       NA         NA     138 - 1    34 - 8
//    38400       NA         NA     103 - 1   207 - 1
// Please note that the UART consumes about all CPU resources
//  when WAIT_ONE*PRESCALER<100.

/* Communication parameters. The WAIT_ONE definiton has to be changed according to equation 2-1 in the application note. */
#define WAIT_ONE             103       //!< Half bit period compare setting. See the application note for calculation of this value. Make sure timer prescaler is set to the intended value.
#define PRESCALER             8       //!< Prescaler setting. Must be set according to the baud rate setting.

/* Port and pin settings. */
#define SW_UART_RX_PIN_NUMBER    2       //!< Set pin number for communication.
#define SW_UART_RX_PORT          PORTD   //!< Set port for communication.
#define SW_UART_RX_PIN           PIND    //!< Set pin for communication.
#define SW_UART_RX_DDR           DDRD    //!< Data direction register. Not available for high voltage ports.

#define TRANSMIT_DELAY        70    //!< Cycles from the start bit is sent (from UART_transmit) to the timer is started plus cycles in the timer interrupt before first data bit is sent.
#define RECEIVE_DELAY         76    //!< Cycles from the start bit is detected to the timer is started plus cycles in timer interrupt before first data bit is received.

#define WAIT_ONEHALF          (WAIT_ONE + WAIT_ONE/2)

#define TIMER_PRESCALER_CONTROL_REGISTER    TCCR0 //!< Define the timer control register according to the timer used for the UART.
#if (PRESCALER == 1)
  #define START_UART_TIMER()     (TIMER_PRESCALER_CONTROL_REGISTER |= (1<<CS00))  //Needs to be change if a different timer is used. Please refer to datasheet.
#define STOP_UART_TIMER()      (TIMER_PRESCALER_CONTROL_REGISTER &= ~(1<<CS00))
#elif (PRESCALER == 8)
  #define START_UART_TIMER()     (TIMER_PRESCALER_CONTROL_REGISTER |= (1<<CS01))
  #define STOP_UART_TIMER()      (TIMER_PRESCALER_CONTROL_REGISTER &= ~(1<<CS01))
#else
  #error PRESCALER must be set to 1 or 8
#endif

#if ( ((WAIT_ONEHALF-(RECEIVE_DELAY/PRESCALER)) > 255) || ((WAIT_ONE) > 255))
  #error WAIT_ONE is set too high. Try to increase prescaler or use a higher baud rate.
#endif

#if ( (WAIT_ONE) < (100/PRESCALER))
  #error A too high baud rate is used. Please check the PRESCALER and WAIT_ONE setting.
#endif

/* Pin macros.  */
//#define INITIALIZE_UART_PIN()   ( SW_UART_RX_PORT &= ~(1<<SW_UART_RX_PIN_NUMBER) )    //!< Clear port.
#define READ_UART_RX_PIN()         ( SW_UART_RX_PIN & (1<<SW_UART_RX_PIN_NUMBER) )

/* Macros for standard AVR ports. */
#define SET_UART_TX_PIN()          ( SW_UART_TX_DDR &= ~(1<<SW_UART_TX_PIN_NUMBER) )     //!< Tri-state pin.
#define CLEAR_UART_TX_PIN()        ( SW_UART_TX_DDR |= (1<<SW_UART_TX_PIN_NUMBER) )      //!< Set pin output low.

/* UART interrupt vectors definitions. */
#define SW_UART_TIMER_COMPARE_INTERRUPT_VECTOR  TIMER0_COMP_vect      //!< UART compare interrupt vector.

/* Timer macros. These are device dependent. */
#define CLEAR_UART_TIMER_ON_COMPARE_MATCH()     (TCCR0 |= (1<<WGM01))                             //!< Set timer control register to clear timer on compare match (CTC).
#define SET_UART_TIMER_COMPARE_WAIT_ONE()       (OCR0 = WAIT_ONE)                                 //!< Sets the timer compare register to one period.
#define SET_UART_TIMER_COMPARE_START_TRANSMIT() (OCR0 = WAIT_ONE - (TRANSMIT_DELAY/PRESCALER))    //!< Sets the timer compare register to the correct value when a transmission is started.
#define SET_UART_TIMER_COMPARE_START_RECEIVE()  (OCR0 = WAIT_ONEHALF - (RECEIVE_DELAY/PRESCALER)) //!< Sets the timer compare register to the correct value when a reception is started.
#define CLEAR_UART_TIMER()                      (TCNT0 = 0x00)
#define ENABLE_UART_TIMER_INTERRUPT()           (TIMSK |= (1<<OCIE0))
#define DISABLE_UART_TIMER_INTERRUPT()          (TIMSK &= ~(1<<OCIE0))
#define CLEAR_UART_TIMER_INTERRUPT_FLAG()       (TIFR &= ~(1<<OCF0)) //(TIFR = (1<<OCF0))


/* Flag macros. */
#define SET_FLAG(flag_register,flag_bit)    ( (flag_register) |= (1 << (flag_bit)) )  //!< Use this macro when setting a flag in a register.
#define CLEAR_FLAG(flag_register,flag_bit)  ( (flag_register) &= ~(1 << (flag_bit)) ) //!< Use this macro when clearing a flag in a register.
#define READ_FLAG(flag_register,flag_bit)   ( (flag_register) & (1 << (flag_bit)) )   //!< Use this macro when reading a flag in a register.


//extern volatile uint8_t SW_UART_status;         //!< Byte holding status flags.

//Use this to put the UART_counter in a dedicated register. Remember to comment out the previous declarations (also in the
//declaration in single_wire_UART.c) of the counter variable and to lock register 15 when this is used. This is done under
//the code section in the C/C++ compiler category in the IAR options menu.
//__no_init __regvar volatile uint8_t SW_UART_status @ 15;

//Use this to put the UART_status in a dedicated register. Remember to comment out the previous declaration of the
//counter variable and to lock register 14 when this is used. This is done under the code section in the C/C++ compiler
//category in the IAR options menu.
//__no_init __regvar volatile uint8_t UART_counter @ 14;

//Use this to put the UART_status in a general purpose io register. Check device datasheet to find adress of GPIO register if available
//(set to 0x1E here). Comment out the previos declarations (also in the declaration in single_wire_UART.c) of the status variable
//when the GPIO register is used.
//__io __no_init static volatile uint8_t SW_UART_status @ 0x1E;


/* Global UART functions. */

//void    SW_UART_Transmit(uint8_t);  //!< Transmit one byte.
//uint8_t SW_UART_Receive(void);      //!< Receive one byte.
