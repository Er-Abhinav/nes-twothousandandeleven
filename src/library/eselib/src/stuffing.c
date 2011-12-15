/*
 * stuffing.c
 *
 *  Created on: Dec 15, 2011
 *      Author: r0726179
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifndef NULL
	#define NULL						((void *) 0)
#endif

/**
 * Adds <addBits> to the <bit> counter
 * if bit sizes overflows a byte, the <byte>
 * counter will be incremented
 */
# define _addBitTo_bitByte_(addBits, bitByte, outBitByte)	\
  do {						\
	  outBitByte.bit = bitByte.bit + addBits; 	\
	  outBitByte.byte = bitByte.byte; \
    while (outBitByte.bit >= 8) {			\
    	outBitByte.bit -= 8;			\
    	outBitByte.byte++;				\
    }						\
  } while (0)


typedef struct bit_byte {
	uint8_t bit;
	uint8_t byte;
} bit_byte_t;

#define STUFF_DATATYPE_BITLENGTH	8
#define STUFF_CYCLE					4
/**
 * @var stuffBitLength[in]	BIT count that should be stuffed,
 * 								NULL in case of destuffing
 * @var destuffByteLength[in]	BYTE count that shoud be destuffed,
 * 								NULL in case of stuffing
 * @var *inBuffer[in]		input Buffer
 * @var *outBuffer[out]		output Buffer
 * @return resulting count of BYTES which were converted
 *
 * @pre		outBuffer has to be cleared
 *
 * sizeof(inBuffer) !=
 * Little endianness == Least significant Bit first
 *  (Byte 0)      (Byte 1)
 *  [LSB,,,,,,,]  [,,,,,,,MSB]
 */
uint8_t stuff(uint16_t stuffBitLength, uint8_t destuffByteLength,
				uint8_t *inBuffer, uint8_t *outBuffer) {
	uint16_t bitsToScan = 0; //Sum of bits scanned
	bit_byte_t scan  = { 0, 0 }; //Bit counter of the sanning byte
	bit_byte_t write = { 0, 0 }; //Sum of bytes scanned
	uint8_t equalBits = 0;
	uint8_t lastBitState = 0xFF, bitState = 0;

	if((stuffBitLength) && (destuffByteLength))
		return 0;


	bitsToScan = (stuffBitLength) ? stuffBitLength : bitsToScan;
	bitsToScan = (destuffByteLength) ? 8*destuffByteLength : bitsToScan;

	for (bitsToScan = bitsToScan; bitsToScan > 0; bitsToScan--) {
		bitState = ((inBuffer[scan.byte] & (1 << scan.bit)) >> scan.bit);

		outBuffer[write.byte] |= (bitState << write.bit);
		_addBitTo_bitByte_(1, write, write);

		if (bitState != lastBitState) {
			equalBits = 0;
		} else if (++equalBits >= STUFF_CYCLE - 1) {
			equalBits = 0;
			if(stuffBitLength) {
				//Stuffing
				outBuffer[write.byte] |= ((bitState ^ 1) << write.bit);
				_addBitTo_bitByte_(1, write, write); //add a bit
			}
			if(destuffByteLength) {
				//Destuffing
				_addBitTo_bitByte_(1, scan, scan); //drop one bit
				bitState = ((inBuffer[scan.byte] & (1 << scan.bit)) >> scan.bit);
			}
		}
		lastBitState = bitState;
		_addBitTo_bitByte_(1, scan, scan);
		if((destuffByteLength) &&
				(scan.byte == destuffByteLength-1)) {
			//Destuffing drop remaining bits
			if(!write.bit) break;
		}
	}
	if (write.bit > 0) {
		//Check if there are left bits in a separate byte
		write.bit = 0;
		write.byte++;
	}
	return write.byte;
}
