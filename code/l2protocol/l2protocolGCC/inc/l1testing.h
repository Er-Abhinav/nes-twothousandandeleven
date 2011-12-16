#ifndef L1TESTING
#define L1TESTING

#include <stdint.h>

void __onSWUSARTStartBitDetected();

void __onSWUSARTByteTransmitted(uint8_t data);

void __onSWUSARTByteCorrupted(uint8_t dataSent,
                              uint8_t dataReceived);


void __onHWUSARTByteReceived(uint8_t data);

void __onHWUSARTByteTransmitted(uint8_t data);

void __onHWUSARTByteCorrupted(uint8_t dataSent,
                              uint8_t dataReceived);

#endif
