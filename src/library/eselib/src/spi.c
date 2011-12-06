/*======================================================================*/
/** @file 	spi.c
 * @brief 	SPI implementation Module				*/
/* Project:	Atmega128 timer driver					*/
/* File:        spi.c							*/
/* MCU          Atmel AVR						*/
/* Revision     $Revision: 1.0 $					*/
/* Date:        $Date: 2009/10/13 $					*/
/* Author(s):   Nick Mayerhofer						*/
/*======================================================================*/
#include "spi.h"


/*======================================================================*/
/* C O N S T A N T S  (place const in flash)                            */
/*======================================================================*/

/*======================================================================*/
/* G L O B A L  V A R I A B L E S  (SRAM)                               */
/*======================================================================*/
static void (*pFuncSIG_SPI)(void) = 0;	//!< @var Function pointer to attach a function to the SPI-ISR


/*======================================================================*/
/* I N T E R R U P T S                                                  */
/*======================================================================*/

/** Serial Transfer Complete
 * ISR Attaching System
*/
SIGNAL(SIG_SPI) {
	//!Checking for attached function
	if(pFuncSIG_SPI)
		pFuncSIG_SPI();
}


/*======================================================================*/
/* S U B R O U T I N E S                                                */
/*======================================================================*/
void spiAttach(void (*pAttachThis)(void) ){
	pFuncSIG_SPI = pAttachThis;
	if(pAttachThis) {
		SET_BIT(SPCR, SPIE);
	} else {
		CLEAR_BIT(SPCR, SPIE);
	}
}


/*======================================================================*/
/* M A I N   R O U T I N E S                                            */
/*======================================================================*/
void spi_MasterInit(void) {
	//Set MOSI and SCK output
	SPI_DDR |= (1<<DD_MOSI)| (1<<DD_SCK)| (1<<DD_SS);
	SET_BIT(SPI_PORT, DD_SS); //Disable Slave Select
	CLEAR_BIT(SPI_DDR, DD_MISO);
	
	SET_BIT(SPCR, CPHA); //Phasen Sampling
	//Enable SPI, Master set clock rate
	SET_BIT(SPCR, SPR0); //clock
	//SET_BIT(SPCR, SPR1);
	SPCR |= (1<<SPE) | (1<<MSTR); 
	SPCR |= (1<<CPOL) | (1<<DORD); //Ruhe Clock Pegel, Data Order
	
	//SET_BIT(SPCR, SPIE);
	
}


uint8_t spi_MasterTransmit(char cData) {
  /* Start transmission */
  SPDR = cData;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF))); //Variante ohne Interrupt
  return SPDR;
}


void spi_MasterTransmit_noblock(char cData) {
  /* Start transmission */
  SPDR = cData;
}


