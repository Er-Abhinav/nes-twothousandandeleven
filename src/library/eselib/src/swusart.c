#include <stdio.h>
#include "swusart.h"
#include "single_wire_UART.h"

// internal
#define SWUSART_STATE_UNINITIALIZED  0
#define SWUSART_STATE_IDLE           1
#define SWUSART_STATE_WRITE_STARTBIT 2
#define SWUSART_STATE_WRITE_DATABIT  3
#define SWUSART_STATE_WRITE_STOPBIT  4

/* Communication parameters. */
static volatile uint8_t   UART_Tx_data;     //!< Byte holding data being transmitted.
static volatile uint8_t   UART_Rx_data;     //!< Byte holding data being received.
static volatile uint8_t   UART_Tx_buffer;   //!< Transmission buffer.
static volatile uint8_t   UART_Rx_buffer;   //!< Reception buffer.

enum _swUart_state {
	UNINITIALIZED = -1,
	IDLE = 0,
	WRITE_STARTBIT,
	WRITE_DATA,
	WRITE_STOP,
	FINISH
};

enum _swUart_finish_states {
	UNKNOWN = -1,
	BUSY,
	GOT_BUS,
	ARBITRATION_LOST
};

volatile uint8_t SW_UART_status = 0;
volatile uint8_t UART_counter;

/*! \brief  Enable the UART.
 *
 *  This function initialize the timer and buffers,
 *  and enables the external interrupt to sense
 *  for incoming data. It is important to run this
 *  function before calling any of the other UART
 *  functions. Received data will automatically be
 *  put in the RX buffer.
 *
 *  \note   The UART can be stopped by disabling the
 *          timer and external interrupt.
 */
void SW_UART_Enable(void)
{
  //Tri-state communication pin.
  INITIALIZE_UART_PIN();

  SW_UART_status = 0x00;
  UART_counter = UART_STATE_IDLE;
  CLEAR_UART_TIMER_ON_COMPARE_MATCH();

  //Set up interrupts
  INITIALIZE_UART_EXTERNAL_INTERRUPT2(); //Set up the external interrupt to wait for incoming start bit on correct pin.
  CLEAR_UART_EXTERNAL_INTERRUPT2_FLAG(); //Clear flag in case it is already set for some reason.
  ENABLE_UART_EXTERNAL_INTERRUPT2();
}


/*! \brief  Disable the UART.
 *
 *  This function disables the UART by disabling the timer
 *  interrupt and the external interrupt.
 */
void SW_UART_Disable()
{
  SW_UART_status = 0x00;
  DISABLE_UART_TIMER_INTERRUPT();
  DISABLE_UART_EXTERNAL_INTERRUPT();
  STOP_UART_TIMER();
}

/*! \brief  Transmit one byte.
 *
 *  This function sends one byte of data by
 *  putting it in the TX data and initialize a
 *  transmission if state is IDLE (UART_counter == 0)
 *
 *  \note   The SW_UART_TX_BUFFER_FULL flag must be
 *          0 when this function is called, or data in
 *          transmit buffer will be lost.
 *
 *  \param  data  Data to be sent.
 */
void SW_UART_Transmit(uint8_t data)
{
  SET_FLAG( SW_UART_status, SW_UART_TX_BUFFER_FULL );
  UART_Tx_buffer = data;

  //Start transmission if no ongoing communication.
  if( UART_counter == UART_STATE_IDLE )
  {
	 PORTC = 0x01;
    //Copy byte from buffer and clear buffer full flag.
    UART_Tx_data = UART_Tx_buffer;
    CLEAR_FLAG( SW_UART_status, SW_UART_TX_BUFFER_FULL );

    DISABLE_UART_EXTERNAL_INTERRUPT2();
    CLEAR_UART_PIN();                         //Write start bit.
    UART_counter = TRANSMIT_FIRST_DATA;       //Update counter so the 1. data bit is the next bit to be transmitted.
    CLEAR_UART_TIMER();                       //Clear timer.
    SET_UART_TIMER_COMPARE_START_TRANSMIT();  //Set timer compare value.
    CLEAR_UART_TIMER_INTERRUPT_FLAG();        //Make sure timer interrupt flag is not set.
    ENABLE_UART_TIMER_INTERRUPT();
    START_UART_TIMER();
    PORTC |= 0x02;
  }
}





enum _swUart_state _sw_uart_state = SWUSART_STATE_UNINITIALIZED;

// events
void(*onStartBitDetected)(void) = 0;

// methods
void SWUSART_initialize(uint8_t mode,
                        uint32_t baud,
                        void(*startBitDetectedCallback)()
                        ) {

    onStartBitDetected = startBitDetectedCallback;

    //Set up interrupts
    INITIALIZE_UART_EXTERNAL_INTERRUPT2();
    CLEAR_UART_EXTERNAL_INTERRUPT2_FLAG();
    ENABLE_UART_EXTERNAL_INTERRUPT2();

    // initialize hw usart
    _sw_uart_state = IDLE;
} // end of HWUSART_initialize

void SWUSART_writeByte (uint8_t data) {
	SW_UART_Enable();
    // trigger write for sw usart
	SW_UART_Transmit(data);
} // end of SWUSART_writeByte


//-----------
// internal -
//-----------
ISR(SW_UART_EXTERNAL_INTERRUPT2_VECTOR) {
    onStartBitDetected();
}

//ISR(SW_UART_EXTERNAL_INTERRUPT_VECTOR) {
//
//    //Make sure bit is low.
//    if(!READ_UART_PIN()) {
//	    onStartBitDetected();
//    } // end if
//} // end of SWUSART_externalISR
