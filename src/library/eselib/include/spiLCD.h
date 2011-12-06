/*======================================================================*/
/* Project:	SPI LCD Display Driverr					*/
/* MCU          Atmel AVR						*/
/**@file	spiLCD.h
   @author 	Nick Mayerhofer
   @version 	1.5
   @date 	2009/11/24						*/
/*======================================================================*/
#ifndef __SPILCD_H__
#define __SPILCD_H__

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "global.h"
#include "spi.h"
#include "ringBuff.h"
#include <avr/delay.h>

/*======================================================================*/
/* H A R D W A R E  D E F I N I T I O N                                 */
/*======================================================================*/
#define LCD_PORT		PORTB
#define LCD_ENABLE_PIN		PB0

//LCD Defines
#define LCD_WIDTH		20
#define LCD_HIGH		4
#define LCD_FONTWIDTH	5 //in dots
#define LCD_ADDR_PERLINE	0x20 //5dot 4line 20chars
//#define LCD_ADDR_PERLINE	0x40 //6dot 2line 20chars
//#define LCD_ADDR_PERLINE	0x40 //6dot 2line 26chars

#define LCD_MEM_ALLDIRTY	0x0F
#define LCD_INIT_CHAR		0x1F
#define LCD_READ_BIT		0x20
#define LCD_BUSYF_MASK		0x80
#define LCD_CLEAR_DISP		0x01
#define LCD_RETURN_HOME		0x02
#define LCD_FUNCTION_SET	0x34
#define LCD_FUNCTION_4LINE	0x09
#define LCD_FUNCTION_CLEAR	0x30
#define LCD_DISP_ON		0x0C //Disp On, Cursor Off
//#define LCD_DISP_ON		0x0F //Disp On, Cursor On, Blink On

#define LCD_SET_DDRAMADR	0x80
#define LCD_SET_CGRAMADR	0x40
#define LCD_SET_SEGRAMADR	0x40
#define LCD_HOME		0x00

//Define Options
#define LCD_LEFT	-1
#define LCD_RIGHT	+1

//LCD-chars
#define LCD_CHAR_SPACE		0x20
#define LCD_CHAR_MINUS		0x2D
#define LCD_CHAR_0		0x30
#define LCD_CHAR_A		0x41
#define LCD_CHAR_a		0x61
//LCD-special Chars
#define LCD_CHAR_POW0		0x80
#define LCD_CHAR_ONELINE	0x1F
#define LCD_CHAR_BLOCK		0x1F

//LCD-Error Codes
#define LCD_LINE_OUT_OF_RANGE		1
#define LCD_START_OUT_OF_RANGE		2
#define LCD_ADDRESS_OUT_OF_RANGE	-1
#define LCD_DIRECTION_UNKNOWN		3
#define LCD_SYNC_DATA_DROPED		-5

// const char tempStr PROGMEM = "";

/*======================================================================*/
/* N U M B E R  &  B A S E                                              */
/*======================================================================*/
/** @struct LCDData
 *  Struct needed to send raw Data to display
*/
typedef struct {
    uint8_t dat; //!<Data to be send
    struct {unsigned char valid:1,	//!< Checked if Data received and written to struct.dat
    			 RW:1,		//!< R/W Bit
    			 RS:1,		//!< RS (Register select)
			 :5;
    } opt; //!< Setup bits
}volatile LCDData;


typedef struct{
    uint8_t lines[8];
}mychar_t;


/**@enum LcdState Lcd Statemachine
 * Contains state information of the LCD Statemachine
*/
enum LcdState {
	SM_Ready,		//!< SM is waiting
	SM_Sent_BusyInit,	//!< SM Sent the busy flag and waits for transmitions to receive the Busyflag
	SM_Wait_BusyData,	//!< SM requested busy data and process them
	SM_BufferEmpty		//!< Ringbuffer is empty, no action
} volatile LcdSM; //!< CD Statemachine saves the current state here


extern volatile struct RB_t ioBuffer; //!< @struct Output Buffer

/*======================================================================*/
/* D E C L A R A T I O N S                                              */
/*======================================================================*/

/**
 * Reads data from Buffer and sends it <br>
 * Function will be atteched to the SPI ISR
 * @pre 
 	- Ringbuffer has to be initialized (LcdBufferInit)
	- LcdHandleBuffer handler has to be attached to the SPI_INT Routine
*/
void _LcdHandleSpiIsrSM(void);

/** Writes a char to the RB
 * @param[in] dataa
*/
void _LcdSpiBufferWrite(char dataa);

/**
 * Reads a Byte Raw from Display and returns it
 * @return receivedChar
 * @retval receivedChar Currently received Byte
*/
uint8_t _LcdReadRaw(LCDData *data);

/*======================================================================*/
/* L O W   L E V E L   F U N C T I O N S                                */
/*======================================================================*/

/**  Prepares the data for the display and writes them to the Buffer
 * @pre Lcd-Buffer has to be initialized (LcdInit())
*/
void _LcdWriteToBuffer(LCDData *data);


/** Minimal \a pow function for positive base and exponent. There's a domain error if base is zero. There's a range error if there's an overflow. 
 * @return returns base raised to the exp power.
*/
uint16_t _myPow(uint16_t base, uint8_t exponent);


/** Writes raw data to Display<br>
 *@par Usage: 
	-Create a instance of LCDData*, fill it with data and call function
 *@param[in] data *Data header for sending
*/
void LcdWriteRaw(LCDData *data);

/** Sends a Clear Command to the Display
 * @pre LCD has to be initialized (LcdInit())
 * @return exitState
*/
int8_t _LcdClear(void);


/** Initialize:
 * 	- SPI Module,
 *	- LCD Ringbuffer and the
 *	- LCD
 * .
 Attaches the ISR-handle function to the SPI
 * @pre 
 	- GIE (Global Interrup Enable) must be set
 	- If you wanna see something on the display the contrast and the background light has to be set
 * @post 
 	- SPI Module must be activated
*/
void LcdInit(void);


/** Sets the LCD Cursor to addr
 * @pre LCD has to be initialized (LcdInit())
 * @param[in] addr Address for curser to be set
*/
void _LcdSetDdramAdr(uint8_t addr);

/**
 * Checks Range
 * @since Version 1.2
 * @param[in] zeile	row to be checked
 * @param[in]	start	coloum to be checked
 * @return Returns 0 if ok or one of the following errorcodes
 	- LCD_LINE_OUT_OF_RANGE
	- LCD_START_OUT_OF_RANGE
*/
int8_t _LcdCheckRange(uint8_t zeile, uint8_t start);

/*======================================================================*/
/* H I G H   L E V E L   F U N C T I O N S                              */
/*======================================================================*/

/** Clears the Lcd-Buffer
 * @return exitState
*/
int8_t LcdClear(void);

/**
 * Clears a Line starting from start
 * @since Version 1.2
 * @param[in] zeile	row to be cleard
 * @param[in]	start	coloum starting to clear till end of line
 * @return Returns 0 if successfull or one of the following errorcodes
 	- LCD_LINE_OUT_OF_RANGE
	- LCD_START_OUT_OF_RANGE
*/
int8_t LcdClearThis(uint8_t zeile, uint8_t start);

/** Writes the Lcd-Buffer information to Display
 * @pre Lcd hase to be initialized
*/
int8_t LcdSync(void);

/**
 * Writes a special Char to the Display<br>
 * valid address range: 0x00 to 0x07
 * @par Usage:
 *	- Fill a mychar_t with line Informations. Least segnificatn 5 Bits are relevant.<br>
 *	- Set all empty lines to NULL.
 @param[in] c mychar_t with line Informations
 @param[in] address ASCII address. Range 0x00 to 0x07
 @return The Exit Status
*/
int8_t LcdSetMyChar(mychar_t c, uint8_t address);

/** \defgroup LcdDisps Description for Lcd-Display write methodes
 * All LcdDisp methodes work with the Lcd-Buffer and manipulate it. <br>To take changes place call LcdSync().<p>
 * If the chosen position of the output \a overflows column range, output will be cut off and an errorcode will be returned.
 * @pre Display has to be initialized (LcdInit());
 * @param[in] zeile	row to print out the output
 * @param[in]	start	start column for the output
 * @return Returns 0 if successfull or one of the following errorcodes
 	- LCD_LINE_OUT_OF_RANGE
	- LCD_START_OUT_OF_RANGE
	- LCD_ADDRESS_OUT_OF_RANGE
 * @{
*/
/**Writes an 8-bit signed int in decimal in the LCD-Buffer*/
int8_t LcdDispSInt(int8_t i, uint8_t zeile, uint8_t start);
/**Writes an 8-bit unsigned integer in decimal in the LCD-buffer*/
int8_t LcdDispUInt(uint8_t i, uint8_t zeile, uint8_t start); 
/**Writes a 8-bit unsigned integer in hexadecimal in the LCD-Buffer*/
int8_t LcdDispHex (uint8_t i, uint8_t zeile, uint8_t start);
/** Writes a Char in the LCD-Buffer*/
int8_t LcdDispChar(uint8_t c, uint8_t zeile, uint8_t start);
/** Writes a String in the LCD-Buffer*/
int8_t LcdDispString(uint8_t *s, uint8_t zeile, uint8_t start);
/** Writes a String from PROGMEM to LCD-Buffer*/
int8_t LcdDispString_P(const char *s, uint8_t zeile, uint8_t start);
/** @} */

/**
 * Shifts a hole line leftward or rightward
 * and fills the new char with a space
 * @pre Lcd-Buffer has to be initialized (LcdInit())
 * @param[in] direction	LCD_LEFT or LCD_RIGHT
 * @param[in] line	line number from 0 to (LCD_HIGH-1)
 * @since Version 1.1
 * @return Returns 0 if successfull or one of the following errorcodes
      - LCD_LINE_OUT_OF_RANGE
      - LCD_DIRECTION_UNKNOWN
*/
int8_t LcdLineShift(int8_t direction, uint8_t line);

/*======================================================================*/
/* M A C R O S                                                          */
/*======================================================================*/


#endif
