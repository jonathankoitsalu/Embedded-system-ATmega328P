#include <avr/io.h>
#include <util/delay.h>
#include "stdint.h"
#include "led.h"

void blinkLed() //blinks led blue.
{
    while(1){
    toggleBit(&PORTB, PB1); 
    delay(500);
    }
}

void delay(unsigned long milliseconds) 
{
    while(milliseconds--){_delay_ms(1);}
}

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
