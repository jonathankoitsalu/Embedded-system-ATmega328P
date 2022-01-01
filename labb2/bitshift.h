#ifndef _BITSHIFT_H_
#define _BITSHIFT_H_

#include "stdint.h"

typedef enum{
    INPUT,
    OUTPUT
} Direction;

void toggleBit(uint8_t*, uint8_t);
void setBitHigh(uint8_t*, uint8_t);
void setBitLow(uint8_t*, uint8_t);
void pinDirection(uint8_t*, Direction, uint8_t);

#endif // _BITSHIFT_H_