#include <avr/io.h>
#include "bitshift.h"

void buttonInit(){
    pinDirection(&DDRB, INPUT, PB0);
    setBitHigh(&PCICR, PCIE0); // enable interrupts
	setBitHigh(&PCMSK0, PCINT0); // enable interrupt PB0
}