#include <stdio.h>
#include "swusart.h"



/* Communication parameters. */
static volatile uint8_t   UART_Tx_data;     //!< Byte holding data being transmitted.
static volatile uint8_t   UART_Rx_data;     //!< Byte holding data being received.

typedef struct _sw_uart_s {  //@TODO move to header
	volatile _sw_uart_state_t state;
	volatile swUart_finish_state_t bus_state;

} _sw_uart_t;

typedef struct _sw_uart_isr {  //@TODO move to header
	volatile uint8_t bitCount;			//!< temp bit counter for ISR
	volatile uint8_t last_bit_sent = 0;	//!< bool that the last bit was sent
} _sw_uart_isr_t;

volatile _sw_uart_t sw_uart = {UNINITIALIZED, UNKNOWN};
volatile _sw_uart_isr_t _swUartISR = {0};

int8_t SW_UART_Enable(void){
  //Tri-state communication pin.
  INITIALIZE_UART_PIN();

  CLEAR_UART_TIMER_ON_COMPARE_MATCH();
  if (sw_uart.state  <= 0) {
	  sw_uart.state = IDLE;
  } else {
	  return -1;
  }
  return 0;
}


/*! \brief  Disable the UART.
 *
 *  This function disables the UART by disabling the timer
 *  interrupt and the external interrupt.
 */
void SW_UART_Disable()
{
  DISABLE_UART_TIMER_INTERRUPT();
  STOP_UART_TIMER();
  sw_uart.state = DISABLED;
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
int8_t swUart_arbitrate(uint8_t data) {
  UART_Tx_data = data;

  if (sw_uart.state != IDLE)
	  return BUSY;

  //Start transmission if no ongoing communication.
  if( sw_uart.state == IDLE )
  {
    //Copy byte from buffer and clear buffer full flag.
    //CLEAR_FLAG( SW_UART_status, SW_UART_TX_BUFFER_FULL );

    CLEAR_UART_PIN();                         //Write start bit.
    //Reset arbitration Variables
    sw_uart.state = WRITE_STARTBIT;
    sw_uart.bus_state = UNKNOWN;
    sw_uart.bitCount = 0;
    _swUartISR.last_bit_sent = 0;
    CLEAR_UART_TIMER();                       //Clear timer.
    SET_UART_TIMER_COMPARE_START_TRANSMIT();  //Set timer compare value.
    CLEAR_UART_TIMER_INTERRUPT_FLAG();        //Make sure timer interrupt flag is not set.
    ENABLE_UART_TIMER_INTERRUPT();
    START_UART_TIMER();
    //@TODO polling bus enabling -> make timer INT, send_startbit??
  }
  return 0;
}


/*! \brief  Timer compare interrupt service routine
 *
 *  This interrupt is triggered when the compare
 *  register equals the timer. It increments the
 *  counter and handles the transmission and
 *  reception of each bit.
 *
 *  \note   The WAIT_ONE define has to be less than
 *          the maximum time spent in this routine
 *          so an interrupt is not missed.
 */
ISR(SW_UART_TIMER_COMPARE_INTERRUPT_VECTOR) {
	SET_UART_TIMER_COMPARE_WAIT_ONE(); //Set timer compare value to trigger the ISR once every bit period.

	//Sample bit by checking the value on the UART pin:

	uint8_t bit_in = 0, bit_out = 0;

	switch(sw_uart.state) {
	case READY:
		sw_uart.state = ERROR;
		break;
	case WRITE_STARTBIT:
		//@pre bus has to be FREE or UNKNOWN status
		PORTA = ~(sw_uart.bitCount++); //Increment bitcount
		//@TODO set new timing
		sw_uart.state = READ_DATA;
		break;
	case WRITE_DATA:

		if(bit_out) SET_UART_PIN();
		else CLEAR_UART_PIN();
		sw_uart.state = READ_DATA;
		break;
	case READ_DATA:
		sw_uart.state = WRITE_DATA;
		bit_in = (READ_UART_PIN()) ? 1 : 0;
		if (sw_uart.bitCount == 1) {
			asm("nop;");
		}

		if (sw_uart.bitCount == 7) {
			_swUartISR.last_bit_sent = UART_Tx_data & 0x01;
			sw_uart.state = WRITE_STOP;
		}

		//If to receive data bit -> Copy received bit into Rx_data.
	    if(sw_uart.bitCount <= 7) {
	      UART_Rx_data = ( UART_Rx_data >> 1 ); //Right shift RX_data so the new bit can be masked into the Rx_data byte.
	      if(bit_in) UART_Rx_data |= 0x80;               //Set MSB of RX data if received bit == 1.
	    }
		break;
	case WRITE_STOP:
		//@TODO write stop bit
		if(_swUartISR.last_bit_sent) {
			sw_uart.bus_state = GOT_BUS;
		} else {
			sw_uart.bus_state = ARBITRATION_LOST;
		}
		//@TODO  release bus-pins
		break;
	case FINISH:
		break;
	default:
		break;
	}

}


// events
void(*onStartBitDetected)(void) = 0;

// methods
void SWUSART_initialize(uint8_t mode,
                        uint32_t baud,
                        void(*startBitDetectedCallback)(void)
                        ) {
    onStartBitDetected = startBitDetectedCallback;
    sw_uart.state = DISABLED;
}

void SWUSART_writeByte (uint8_t data) {
	if (SW_UART_Enable() < 0)
		return;
    // trigger write for sw usart
	swUart_arbitrate(data);
} // end of SWUSART_writeByte

