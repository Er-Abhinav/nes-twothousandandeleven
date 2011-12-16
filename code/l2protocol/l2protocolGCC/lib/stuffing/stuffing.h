#include <stdint.h>

/**
 * @var stuffBitLength[in]	BIT count that should be stuffed,
 * 								0 in case of destuffing
 * @var destuffByteLength[in]	BYTE count that shoud be destuffed,
 * 								0 in case of stuffing
 * @var *inBuffer[in]		input Buffer
 * @var *outBuffer[out]		output Buffer
 * @return resulting count of BYTES which were converted
 *
 * @pre		outBuffer has to be set to '\0'
 *
 * Little endianness == Least significant Bit first
 *  (Byte 0)      (Byte 1)
 *  [LSB,,,,,,,]  [,,,,,,,MSB]
 */
uint8_t stuff(uint16_t stuffBitLength,
		      uint8_t destuffByteLength,
			  uint8_t *inBuffer,
			  uint8_t *outBuffer);
