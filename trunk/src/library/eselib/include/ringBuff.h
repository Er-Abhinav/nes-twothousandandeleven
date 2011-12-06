/*======================================================================*/
/* Project:	RingBuffer Driver					*/
/* File:        ringBuff.h						*/
/* Revision     $Revision: 1.2 $					*/
/* Date:        $Date: 2009/10/20 $					*/
/* Author(s):   Nick Mayerhofer						*/
/*======================================================================*/
#ifndef RINGBUFF_H
#define RINGBUFF_H

#include <avr/io.h>
#include <stdio.h>
#include "global.h"

/*======================================================================*/
/* D E F I N E S							*/
/*======================================================================*/
#define Q uint8_t//void*
#define BUFFSIZE 255
#define BOTTOM 0

#define RB_OK	0
#define RB_FULL	5

/** \struct RB_t ringBuff.h 
 *  Structure to control the ringbuffer
*/
struct RB_t{
	Q buf[BUFFSIZE];	//!< buf RingBuffer elements
	uint8_t rbRead;		//!< rbRead Points to the read position
	uint8_t rbWrite;	//!< rbWrite Points to the write Position
	uint8_t rbTuples;	//!< rbTuples is a counter of the containig Elements
};

/*======================================================================*/
/* D E C L A R A T I O N S                                              */
/*======================================================================*/
/**
 * Initialisation of the Ringbuffer
 * @par Usage:
 	Create a Ringbuffer Object prefered as Global: RB_t outpBuffer; <br>
	Initialize the RingB.: RB_Init(&outBuffer);
 * @param[in] f *Pointer to the Ringbuffer struct RB_t*
*/
void RB_Init(struct RB_t *f);

/** Asks if the Buffer is Empty
 * @return returns one if it's empty and zero otherwise
*/
uint8_t RB_IsEmpty(struct RB_t *f);

/**
 * Writes an element to the Ringbuffer
 * @par Usage:
 	Write to buffer with: RB_Write(&outBuffer, Q);  Q is Predefined in the ringBuff.h
 * @param[in] f   *Ringbuffer Structure
 * @param[in] el  The new element to add it
 *`@return Returns RB_OK or RB_FULL
 *
*/
uint8_t RB_Write(struct RB_t *f, Q el);

/** Returns the next element from buffer
 * @param[in] f *RingBuffer Structure
 * @return Next element from ringBuffer
*/
Q RB_Read(struct RB_t *f);

/**
 * Gives the remaining space in the buffer
 * @return Free space from buffer
*/
uint16_t RB_FreeSpace(struct RB_t *f);


#endif
