#include <uart.h>

/** \file uart.c
 * \brief The uart driver
 * \author Martin Perner, 0725782
 * \version 0.9.2
 * \date 2009/10/29
 *
 * Allows transmitting and receiving from/to uart0/1
 * uart1 is non-blocking
 */

/**
 * \brief UART0 Receive handler function
 */
void (*uart_rx_isr_0)(uint8_t);
/**
 * \brief UART1 Receive handler function
 */
void (*uart_rx_isr_1)(uint8_t);

/**
 * \brief This function is used to configure UART0
 * \param mode Mode to configure, use predefined defines for this
 * \param baud Baud rate for the transmission
 * \param *receivefunction handler function which will be called by
 * the ISR
 * \note The \e baud value is used for calculation of the UBRR register. Which 
 * is limited to a maximum of 11-Bit. For the upper baud-rate of your clock look
 * at pp 195 of the ATmega128 datasheet
 *
 * This function is used for configuring UART0 on the target. Supported modes
 * are 5,6,7,8 databits, even/odd/none parity and 1/2 stopbits. 9 databits are
 * \b not supported.
 * The \e baud value is used for calculating the UBRR register as followed 
 * \f$\frac{f_{CPU}}{16*baud}\f$. The UBRR register is limited to 11 bits thus
 * the maximum baud rate is also limited through the clock for the target. For
 * further information look at pp 195 of the ATmega128 datasheet
 */
void configure_uart_0(uint8_t mode, uint32_t baud, void (*receivefunction)(uint8_t))
{

	//disable interrupt and save sreg
	__atomic_start();

	uint16_t ubrr;

	ubrr = (uint8_t)(CPU_CLK/(16*baud))-1;

	//Limit size of baud-rate to 11 Bit
	ubrr = (ubrr > UART_MAX_UBRR) ? UART_MAX_UBRR : ubrr;

	//Set baud-rate
	UBRR0H = (uint8_t)(ubrr>>8);
	UBRR0L = (uint8_t)ubrr;
	
	//Set Asynchronous Operation
	UCSR0C &= ~(1<<UMSEL);

	//Start Set Parity, Bits 5:4 are parity bits
	if((mode & (1<<UPM1)) != 0) {
		UCSR0C |= (1<<UPM1);
	}
	else {
		UCSR0C &= ~(1<<UPM1);
	}
	
	if((mode & (1<<UPM0)) != 0) {
		UCSR0C |= (1<<UPM0);
	}
	else {
		UCSR0C &= ~(1<<UPM0);
	}
	//End Set Parity

	//Start Set Stop-Bits, Bit 3 is stop bit
	if((mode & (1<<USBS)) != 0) {
		UCSR0C |= (1<<USBS);
	}
	else {
		UCSR0C &= ~(1<<USBS);
	}
	//End Set Stop-Bits

	//Start Set Data-Bits,Bits 2:1
	if((mode & (1<<UCSZ1)) != 0) {
		UCSR0C |= (1<<UCSZ1);
	}
	else {
		UCSR0C &= ~(1<<UCSZ1);
	}

	if((mode & (1<<UCSZ0)) != 0) {
		UCSR0C |= (1<<UCSZ0);
	}
	else {
		UCSR0C &= ~(1<<UCSZ0);
	}

	//Third Data-Bits, only needed for 9-Databits
	UCSR0B &= ~(1<<UCSZ2);

	//End Set Data-Bits


	//Enable receiver and transmitter
	UCSR0B |= ((1<<RXEN0)|(1<<TXEN0));

	//null-pointer tx-disable?
	if(receivefunction != NULL) {
		uart_rx_isr_0 = receivefunction;
		UCSR0B |= (1<<RXCIE0);
	}

	//re enable interrupt if it was enabled before
	__atomic_end();


	return;

}


/**
 * \brief This function is used to configure UART1
 * \param mode Mode to configure, use predefined defines for this
 * \param baud Baud rate for the transmission
 * \param *receivefunction handler function which will be called by
 * the ISR
 * \note The \e baud value is used for calculation of the UBRR register. Which 
 * is limited to a maximum of 11-Bit. For the upper baud-rate of your clock look
 * at pp 195 of the ATmega128 datasheet
 *
 * This function is used for configuring UART1 on the target. Supported modes
 * are 5,6,7,8 databits, even/odd/none parity and 1/2 stopbits. 9 databits are
 * \b not supported.
 * The \e baud value is used for calculating the UBRR register as followed 
 * \f$\frac{f_{CPU}}{16*baud}\f$. The UBRR register is limited to 11 bits thus
 * the maximum baud rate is also limited through the clock for the target. For
 * further information look at pp 195 of the ATmega128 datasheet
 */
void configure_uart_1(uint8_t mode, uint32_t baud, void (*receivefunction)(uint8_t))
{



	//disable interrupt and save sreg
	__atomic_start();

	uint16_t ubrr;

	ubrr = (uint8_t)(CPU_CLK/(16*baud))-1;

	//Limit size of baud-rate to 11 Bit
	ubrr = (ubrr > UART_MAX_UBRR) ? UART_MAX_UBRR : ubrr;

	//Set baud-rate
	UBRR1H = (uint8_t)(ubrr>>8);
	UBRR1L = (uint8_t)ubrr;
	
	//Set Asynchronous Operation
	UCSR1C &= ~(1<<UMSEL);

	//Start Set Parity, Bits 5:4 are parity bits
	if((mode & (1<<UPM1)) != 0) {
		UCSR1C |= (1<<UPM1);
	}
	else {
		UCSR1C &= ~(1<<UPM1);
	}
	
	if((mode & (1<<UPM0)) != 0) {
		UCSR1C |= (1<<UPM0);
	}
	else {
		UCSR1C &= ~(1<<UPM0);
	}
	//End Set Parity

	//Start Set Stop-Bits, Bit 3 is stop bit
	if((mode & (1<<USBS)) != 0) {
		UCSR1C |= (1<<USBS);
	}
	else {
		UCSR1C &= ~(1<<USBS);
	}
	//End Set Stop-Bits

	//Start Set Data-Bits,Bits 2:1
	if((mode & (1<<UCSZ1)) != 0) {
		UCSR1C |= (1<<UCSZ1);
	}
	else {
		UCSR1C &= ~(1<<UCSZ1);
	}

	if((mode & (1<<UCSZ0)) != 0) {
		UCSR1C |= (1<<UCSZ0);
	}
	else {
		UCSR1C &= ~(1<<UCSZ0);
	}

	//Third Data-Bits, only needed for 9-Databits
	UCSR1B &= ~(1<<UCSZ2);

	//End Set Data-Bits


	//Enable receiver and transmitter
	UCSR1B |= ((1<<RXEN)|(1<<TXEN));

	//null-pointer tx-disable?
	if(receivefunction != NULL) {
		uart_rx_isr_1 = receivefunction;
		UCSR1B |= (1<<RXCIE);
	}

	//Initialize the buffer
	RB_Init((void*)&uart1_buffer);
	UCSR1B |= (1<<UDRIE);

	//re enable interrupt if it was enabled before
	__atomic_end();


	return;

}


/**
 * \brief Sends a byte on UART
 * \param byte Byte which should be send
 * \note This function is blocking
 * \warning A incoming transmission can be lost!
 *
 * This function sends a byte on the UART0. This function is not thread-safe
 * but interrupt save. Through the connected TX and RX the RX will be disabled
 * while sending a byte. Thus a incoming byte can be lost. The RX will be 
 * re enabled after complete sending of the byte.
 *
 * The function polls before and after the transmit for data register empty.
 * After the transmission it polls also for transmission complete.
 * This allows the function to not disable the interrupt and still can be called
 * in an isr.
 */
void send_byte_0(uint8_t byte)
{
	//busy-wait for data register empty
	while(!(UCSR0A & (1<<UDRE)));
	//disable RX interrupt
	UCSR0B &= ~(1<<RXEN0);
	// Send Data
	UDR0 = byte;
	//busy-wait for data register empty, to minimize the time with interrupt disabled
	while(!(UCSR0A & (1<<UDRE)));
	//busy-wait for transmission complete
	uint8_t tmp;
	tmp = SREG;
	cli();
	while(!(UCSR0A & (1<<TXC0)));
	SREG = tmp;
	//TXC0 reset by writing 1 to TXC0
	UCSR0A = (1<<TXC0);
	UCSR0B |= (1<<RXEN0);
}

/**
 * \brief Writes a byte into the UART1 TX-Buffer
 * \param byte Byte which should be send
 * \note This function is non-blocking
 * \warning A byte can be lost if the buffrer is full!
 *
 * This function writes a byte into the UART1 buffer.
 *
 */
void send_byte_1(uint8_t byte)
{
	RB_Write((void*)&uart1_buffer, byte);
	if((UCSR1A & (1<<UDRE)) != 0) {
		send_to_bus1();
		UCSR1B |= (1<<UDRIE);
	}
}

/**
 * \brief Sends a byte on UART1
 *
 * This function writes a byte from the UART1 buffer.into the TX buffer
 *
 */
void send_to_bus1()
{
	UDR1 = RB_Read((void*)&uart1_buffer);
}

/**
 * \brief Data-Register-Empty buffer isr
 *
 * If the UART-TX-Buffer is empty disable the Data-Register-Empty-Interrupt, else
 * call the function to write a byte to the TX-buffer
 *
 */
SIGNAL(SIG_UART1_DATA)
{
	if(RB_IsEmpty((void*)&uart1_buffer)) {
		UCSR1B &= ~(1<<UDRIE);
	}
	else {
		send_to_bus1();
	}
}

/**
 * \brief Sends a byte encoded in binary on the UART with heading 'b'
 * \param byte Byte which should be send
 * \note This function does not disable interrupts. To ensure that the
 * transmission is not mixed with another transmission the user have to disable
 * interrupts. Further the transfers are blocking
 * \warning Incoming transmission can be lost!
 */
void send_byte_bin_0(uint8_t byte)
{
	send_byte_0('b');
	uint8_t check = 0x80;
	do {
		if((byte & check) != 0) {
			send_byte_0('1');
		}
		else {
			send_byte_0('0');
		}
		check = check>>1;
	}
	while(check != 0x01);
}

/**
 * \brief Sends a byte encoded in binary on the UART with heading 'b'
 * \param byte Byte which should be send
 * \note This function does not disable interrupts. To ensure that the
 * transmission is not mixed with another transmission the user have to disable
 * interrupts. Further the transfers are blocking
 * \warning Incoming transmission can be lost!
 */
void send_byte_bin_1(uint8_t byte)
{
	send_byte_1('b');
	uint8_t check = 0x80;
	do {
		if((byte & check) != 0) {
			send_byte_1('1');
		}
		else {
			send_byte_1('0');
		}
		check = check>>1;
	}
	while(check != 0x01);
}

/**
 * \brief Sends a byte displayed as a decimal over UART1
 * \param byte Byte which should be send
 */
void send_byte_dec_1(uint8_t byte)
{
	uint8_t i=0;
	if(byte >= 100) {
		do {
			i++;
			byte -= 100;
		}
		while(byte >= 100);
		send_byte_1('0'+i);
	}
	if(byte >= 10 || i != 0) {
		i = 0;
		while(byte >= 10) {
			i++;
			byte -= 10;
		}
		send_byte_1('0'+i);
	}
	send_byte_1('0'+byte);
}

/**
 * \brief Sends a word displayed as a decimal over UART1
 * \param byte Word which should be send
 */
void send_byte_dec16_1(uint16_t byte)
{
	uint8_t i=0, print = FALSE;
	if(byte >= 10000 || print) {
		print = TRUE;
		i=0;
		while(byte >= 10000) {
			i++;
			byte -= 10000;
		}
		send_byte_1('0'+i);
	}
	if(byte >= 1000 || print) {
		print = TRUE;
		i=0;
		while(byte >= 1000) {
			i++;
			byte -= 1000;
		}
		send_byte_1('0'+i);
	}
	if(byte >= 100 || print) {
		print = TRUE;
		i=0;
		while(byte >= 100) {
			i++;
			byte -= 100;
		}
		send_byte_1('0'+i);
	}
	if(byte >= 10 || print) {
		i = 0;
		while(byte >= 10) {
			i++;
			byte -= 10;
		}
		send_byte_1('0'+i);
	}
	send_byte_1('0'+byte);
}


/**
 * \brief Sends a byte encoded as hex the UART with heading "0x"
 * \param byte Byte which should be send
 * \note This function does not disable interrupts. To ensure that the
 * transmission is not mixed with another transmission the user have to disable
 * interrupts. Further the transfers are blocking
 * \warning Incoming transmission can be lost!
 */
void send_byte_hex_0(uint8_t byte)
{
	send_byte_0('0');
	send_byte_0('x');
	uint8_t hex;
	hex = (byte>>4)&0x0F;
	hex += '0';//offset 0
	if(hex > '9') {
		hex += 'A'-'9'-1; //offset between '9' and 'A'
	}
	send_byte_0(hex);
	hex = byte & 0x0F;
	hex += '0';
	if(hex > '9') {
		hex += 'A'-'9'-1;
	}
	send_byte_0(hex);

}

/**
 * \brief Sends a byte encoded as hex the UART with heading "0x"
 * \param byte Byte which should be send
 * \note This function does not disable interrupts. To ensure that the
 * transmission is not mixed with another transmission the user have to disable
 * interrupts. Further the transfers are blocking
 * \warning Incoming transmission can be lost!
 */
void send_byte_hex_1(uint8_t byte)
{
	send_byte_1('0');
	send_byte_1('x');
	uint8_t hex;
	hex = (byte>>4)&0x0F;
	hex += '0';//offset 0
	if(hex > '9') {
		hex += 'A'-'9'-1; //offset between '9' and 'A'
	}
	send_byte_1(hex);
	hex = byte & 0x0F;
	hex += '0';
	if(hex > '9') {
		hex += 'A'-'9'-1;
	}
	send_byte_1(hex);

}

/**
 * \brief Sends a string over UART0
 * \param str String which should be send
 */
void send_string_0(char* str)
{
	uint8_t i;
	for(i=0; i < strlen(str); i++) {
		send_byte_0(str[i]);
	}
}

/**
 * \brief Sends a string over UART1
 * \param str String which should be send
 */
void send_string_1(char* str)
{
	uint8_t i;
	for(i=0; i < strlen(str); i++) {
		send_byte_1(str[i]);
	}
}


/**
* \brief Sends a string over UART1
* \param str String which should be send
*/
void send_string_P_1(const char* str)
{
	while (pgm_read_byte(str)) {
		send_byte_1(pgm_read_byte(str++));
	}
}


/**
 * \brief ISR for UART RX
 * \note If an error bit is set, the handler function will not be called
 */
SIGNAL(SIG_USART0_RECV)
{
	uint8_t status, temp;
	status = UCSR0A;
	temp = UDR0;
	if((status & ((1<<FE)|(1<<DOR)|(1<<UPE))) != 0) {
		//send_byte_hex_1(status);
		//error
	}
	else {
		if(uart_rx_isr_0 != NULL) {
			//sei();
			uart_rx_isr_0(temp);
		}
	}
	return;
}

/**
 * \brief Disables the RX-Interrupt of UART0
 *
 * Disables the RX-Interrupt of UART0 and sets the SWUART-Ports for output
 */
void disable_rx_interrupt_uart_0(void)
{
	UCSR1B &= ~(1<<RXEN);
	HWUART0_OPORT  |= ((1<<HWUART0_RXD) | (1<<HWUART0_TXD));
	HWUART0_DDR &= ~(1<<HWUART0_TXD);
	SWUART_DDR |= (1<<SWUART_TXD);
}

/**
 * \brief Sets SWUART to input
 *
 * Configures the SWUART-Pins to input/idle and HWUART for sending
 */
void enable_hwuart_0(void)
{
	SWUART_OPORT  |= ((1<<SWUART_RXD) | (1<<SWUART_TXD));
	HWUART0_DDR |= (1<<HWUART0_TXD);
	SWUART_DDR &= ~(1<<SWUART_TXD);	
}

/**
 * \brief Enables the RX-Interrupt of UART0
 *
 */
void enable_rx_interrupt_uart_0(void) {
	UCSR1B |= (1<<RXEN);
}

/**
 * \brief ISR for UART RX
 * \note If an error bit is set, the handler function will not be called
 */
SIGNAL(SIG_USART1_RECV)
{
	uint8_t status, temp;
	status = UCSR1A;
	temp = UDR1;
	if((status & ((1<<FE)|(1<<DOR)|(1<<UPE))) != 0) {
		//error
	}
	else {
		if(uart_rx_isr_1 != NULL) {
			//sei();
			uart_rx_isr_1(temp);
		}
	}
	return;
}

