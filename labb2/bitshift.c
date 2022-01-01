#include "bitshift.h"
#include <avr/io.h>

void toggleBit(uint8_t* Register, uint8_t bit) 
{
    *Register^=(1<<bit);
}

void setBitHigh(uint8_t* Register, uint8_t bit) 
{
    *Register|=(1<<bit);
}

void setBitLow(uint8_t* Register, uint8_t bit)
{
    *Register&=~(1<<bit);
}

void pinDirection(uint8_t* Register, Direction direction, uint8_t bit) //Sets pindirection. Uses Enums.
{
    if(direction==INPUT){
        setBitLow(Register, bit);
    }
    if(direction==OUTPUT){
        setBitHigh(Register, bit);
    }
}
