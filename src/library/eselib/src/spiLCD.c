/************************************************************************/
/**@file	spiLCD.c
   @brief       SPI LCD Display Driver					*/
/* MCU          Atmel AVR						*/
/* Date:        $Date: 2009/10/16 $					*/
/* Author(s):   Nick Mayerhofer						
 ************************************************************************/
#include "spiLCD.h"

/*======================================================================*/
/* G L O B A L  V A R I A B L E S  (SRAM)                               */
/*======================================================================*/
char lcdMem[LCD_WIDTH][LCD_HIGH]; 	//!< LCD-Buffer which swaps data with display
uint8_t lcdMemDirty=LCD_MEM_ALLDIRTY; 	//!< Dirty Flags for each line of the Display that marks the line for a refresh

volatile struct RB_t ioBuffer;	  	//!< SPI Output Buffer

/*======================================================================*/
/* S U B R O U T I N E S                                                */
/*======================================================================*/
void _LcdHandleSpiIsrSM(void) {
   char inBuffer=SPDR;
   static char nextCommand=0;
	//Entering Statemachine
	switch(LcdSM) {
		case SM_Ready:
			if(!RB_IsEmpty((void*)&ioBuffer) ) {
				nextCommand=RB_Read((void*)&ioBuffer);
				if((nextCommand&LCD_INIT_CHAR) == LCD_INIT_CHAR) {
					LcdSM= SM_Sent_BusyInit;
					spi_MasterTransmit_noblock(LCD_INIT_CHAR| 1<<5);
					break;
				}
				//Send out the command
				spi_MasterTransmit_noblock(nextCommand);
			}else {
				LcdSM= SM_BufferEmpty;
			}
			break;
		case SM_Sent_BusyInit:
			spi_MasterTransmit_noblock(0x00);
			LcdSM=SM_Wait_BusyData;
			break;
		
		case SM_Wait_BusyData:
			if((inBuffer&LCD_BUSYF_MASK) != 0) {  //if LCD-Busy request Busy State again
				spi_MasterTransmit_noblock(LCD_INIT_CHAR| 1<<5); //Busy Flag Abfrage
				LcdSM=SM_Sent_BusyInit;
			} else { //LCD-Ready
				LcdSM= SM_Ready;
				spi_MasterTransmit_noblock(nextCommand);
			}
			break;
		case SM_BufferEmpty:
			break;
	}
}



void _LcdSpiBufferWrite(char dataa) {
	SET_BIT(SPCR, SPIE);
	RB_Write((void*)&ioBuffer, dataa);
}

uint8_t _LcdReadRaw(LCDData *data) {
	uint8_t tmp, out;
	tmp= (LCD_INIT_CHAR|(data->opt.RW<<5)|(data->opt.RS<<6));
	spi_MasterTransmit(tmp); //Send the Read Information
	out = spi_MasterTransmit(0x00);
	out&=0x80;
	return out;
}

/*======================================================================*/
/* L O W   L E V E L   F U N C T I O N S                                */
/*======================================================================*/

void _LcdWriteToBuffer(LCDData *data) {
    uint8_t tmp;
	//Split data and Send
	tmp= LCD_INIT_CHAR;
	tmp|= ((data->opt.RW<<5)|(data->opt.RS<<6));
	//if(altSetup != tmp) {
		_LcdSpiBufferWrite(tmp);
	//}

	tmp=(data->dat&0x0F);
	_LcdSpiBufferWrite(tmp);
	tmp=(data->dat>>4)&0x0F;
	_LcdSpiBufferWrite(tmp);
	
	//SM Handling
	if(LcdSM == SM_BufferEmpty) {
		LcdSM = SM_Ready;
		spi_MasterTransmit_noblock(LCD_INIT_CHAR);
	}
}

uint16_t _myPow(uint16_t base, uint8_t exponent) {
	if(exponent==0) return 1;
	for(exponent=exponent; exponent>1; exponent--)
		base*=base;
	return base;
}


/*======================================================================*/
/* M A I N   R O U T I N E S                                            */
/*======================================================================*/


int8_t _LcdClear(void) {
    LCDData clearData;
	//Return Home
	clearData.opt.RW=0;
	clearData.opt.RS=0;
	clearData.dat=LCD_RETURN_HOME; //Return Home

	_LcdWriteToBuffer(&clearData);	
	clearData.dat=LCD_CLEAR_DISP; //Clear Display
	_LcdWriteToBuffer(&clearData);

	return 0;
}

//script Seite 14
void _LcdSetDdramAdr(uint8_t addr) {
    LCDData tempp;
    	tempp.opt.RW=0;
	tempp.opt.RS=0;
	tempp.dat=LCD_SET_DDRAMADR|addr;
	_LcdWriteToBuffer(&tempp);
}



void LcdInit(void) {
   LCDData initData;
   	//Init Statemachine
   	LcdSM=SM_BufferEmpty;
   	//Init SPI
   	spi_MasterInit();
	//Init RingBuffer
	RB_Init((void*)&ioBuffer);
	//Attach ISR-function
	spiAttach(_LcdHandleSpiIsrSM);
	//spi_MasterTransmit((char)0x00);
	SET_BIT(SPCR, SPIE);
	
	CLEAR_BIT(LCD_PORT, LCD_ENABLE_PIN); //Enable LCD-CS
	_delay_ms(30);
	_delay_ms(30);
	initData.opt.RW=0;
	initData.opt.RS=0;
	initData.dat=LCD_FUNCTION_SET;
	_LcdWriteToBuffer(&initData); //8-Bit, extension Bit RE=1
	initData.dat=LCD_FUNCTION_4LINE;
	_LcdWriteToBuffer(&initData); //4-Line Mode=ON
	initData.dat=LCD_FUNCTION_CLEAR;
	_LcdWriteToBuffer(&initData); //RE=0

	initData.dat=LCD_DISP_ON;
	_LcdWriteToBuffer(&initData);
	_LcdClear(); //Clear
	
	LcdClear();
}

int8_t _LcdCheckRange(uint8_t zeile, uint8_t start) {
	if(zeile>=LCD_HIGH) return LCD_LINE_OUT_OF_RANGE;
	if(start>=LCD_WIDTH) return LCD_START_OUT_OF_RANGE;
	return 0;
}

inline void _LcdtoMem(uint8_t spalte, uint8_t zeile, char z) {
	if(lcdMem[spalte][zeile]!= z) {
	    lcdMem[spalte][zeile]= z;
	    lcdMemDirty|=(1<<zeile);
	}
}

/*======================================================================*/
/* H I G H   L E V E L   F U N C T I O N S                              */
/*======================================================================*/

int8_t LcdClear(void) {
   uint8_t j;
	//Init the Synchron Register
	for(j=0; j<LCD_HIGH; j++) {
		LcdClearThis(j, 0);
	}
	return 0;
}

int8_t LcdClearThis(uint8_t zeile, uint8_t start) {
    uint8_t rett=0;
    uint8_t i;
	rett= _LcdCheckRange(zeile, start);
	if(!rett) {
		for(i=start; i<LCD_WIDTH; i++) {
			//lcdMem[i][zeile]=LCD_CHAR_SPACE;//0xDD;
			_LcdtoMem(i, zeile, LCD_CHAR_SPACE);
		}
	}
	return rett;
}

int8_t LcdSync(void) {
    uint8_t i,j;
    LCDData toBuffer;
    uint8_t rett=0;
    uint8_t jump=1;
	//different handling between in or outside an ISR
	if(RB_FreeSpace((void*)&ioBuffer)<150) {
		/*if(SREG&(1<<7)) {
			//Warte bis genuegend Speicher im Buffer zur Verfuegung steht
			while(RB_FreeSpace((void*)&ioBuffer)<150);
		} else {*/
			return LCD_SYNC_DATA_DROPED;
		//}
	}
	
	toBuffer.opt.RW=0;
	toBuffer.opt.RS=1;
	_LcdSetDdramAdr(LCD_HOME); //Return Home
	
	//Writes all chars to Buffer
	for(j=0; j<LCD_HIGH; j++) {
	      //Linewise Dirty Flag: Branch handling
	      if(lcdMemDirty&(1<<j)) {
		  CLEAR_BIT(lcdMemDirty, j);
		  if(jump) {
		      _LcdSetDdramAdr(LCD_ADDR_PERLINE*j);
		      jump=FALSE;
		  }
		  //Writeout
		  for(i=0; i<LCD_WIDTH; i++) {
			  toBuffer.dat=lcdMem[i][j];
			  _LcdWriteToBuffer(&toBuffer);
		  }
	      } else
		  jump=TRUE;
	}
	return rett;
}


int8_t LcdSetMyChar(mychar_t c, uint8_t address) {
    uint8_t i, tmp;
    LCDData cgRam, writeData;
	//Check the addressrange
	if(address>7) return LCD_ADDRESS_OUT_OF_RANGE;
	//Prepare Data Header
    	cgRam.opt.RS=0;
       	cgRam.opt.RW=0;
       	writeData.opt.RS=1;
       	writeData.opt.RW=0;
       	//Send every line
	for(i=0; i<8; i++) {
		tmp=LCD_SET_CGRAMADR;	//Set CG-Ram Command
		tmp|=(address<<3);	//Set the Address
		tmp|=i;			//Set the Line
		cgRam.dat=tmp;
		writeData.dat=c.lines[i];
		//Send to Display Mem
		_LcdWriteToBuffer(&cgRam);
		_LcdWriteToBuffer(&writeData);
	}
	return 0;
}

int8_t LcdDispSInt(int8_t i, uint8_t zeile, uint8_t start) {
    uint8_t j, rett;
	if(i&0x80) {
		rett=LcdDispChar(LCD_CHAR_MINUS, start, zeile);
		i--;
		i=~i;		
	}else {
		rett=LcdDispChar(LCD_CHAR_SPACE, start, zeile);
	}
	//Transform to each char
	for(j=1; j<=3; j++) {
		rett=LcdDispChar( LCD_CHAR_0+(i%_myPow(10,j))/_myPow(10,j-1), zeile, start+4-j);
	}
	return rett;
}


int8_t LcdDispUInt(uint8_t i, uint8_t zeile, uint8_t start) {
    uint8_t j, rett;
	//Transform to each char
	for(j=1; j<=3; j++) {
		rett=LcdDispChar( LCD_CHAR_0+(i%_myPow(10,j))/_myPow(10,j-1), zeile, start+3-j);
	}
	return rett;
}


int8_t LcdDispHex (uint8_t i, uint8_t zeile, uint8_t start) {
    uint8_t j, tmp, out, rett;
	//Transform to single chars
	tmp=i;
	for(j=1; j<=2; j++) {
		out= tmp%16;
		tmp/=16;
		if(out<10)
			out+=LCD_CHAR_0;
		else 
			out+=LCD_CHAR_A-10;
		rett= LcdDispChar( out, zeile, start+2-j);
	}
	return rett;

}


int8_t LcdDispChar(uint8_t c, uint8_t zeile, uint8_t start) {
    uint8_t rett=0;
	//Check display range
	rett= _LcdCheckRange(zeile, start);
	if(!rett)
		//Write to Buffer
		//lcdMem[start][zeile]=c;
		_LcdtoMem(start, zeile, c);
	return rett;
}

int8_t LcdDispString(uint8_t *s, uint8_t zeile, uint8_t start) {
    uint8_t rett=0;
	while(*s) {
		rett=LcdDispChar(*s++, zeile, start++);
		if(rett) break;
	}
	return rett;
}

int8_t LcdDispString_P(const char *s, uint8_t zeile, uint8_t start) {
  uint8_t rett=0;
    while (pgm_read_byte(s)) {
	rett=LcdDispChar(pgm_read_byte(s++), zeile, start++);
	if(rett) break;
    }
    return rett;
}

int8_t LcdLineShift(int8_t direction, uint8_t line) {
  uint8_t rett=	0;
  uint8_t i;
    //Error Condition
    if(line>=LCD_HIGH) rett=LCD_LINE_OUT_OF_RANGE;
    if( (direction!=LCD_LEFT) && (direction!=LCD_RIGHT) )
      rett= LCD_DIRECTION_UNKNOWN;
    
    //Logic
    if(!rett) {
	for(i=0; i<(LCD_WIDTH-1); i++) {
	  if(direction==LCD_LEFT)
	    _LcdtoMem(i, line, lcdMem[i+1][line]);
	  else
	    _LcdtoMem(LCD_WIDTH-i-1, line, lcdMem[LCD_WIDTH-i-2][line]);
	}
      
	if(direction==LCD_LEFT)
	  _LcdtoMem(LCD_WIDTH-1, line, LCD_CHAR_SPACE);
	else 
	  _LcdtoMem(0, line, LCD_CHAR_SPACE);
    }
   return rett;
}
