#include <avr/io.h>
#include "timer.h"

void timerInit(){
	timer0Init();
	timer2Init();
}

void timer0Init(){
	//set fast PWM mode
	setBitHigh(&TCCR0A, WGM00);
	setBitHigh(&TCCR0A, WGM01);
	setBitLow(&TCCR0B, WGM02);

	//set non-inverting mode
	setBitLow(&TCCR0A, COM0A0);
	setBitHigh(&TCCR0A, COM0A1);

	//set Prescaler 64
	setBitHigh(&TCCR0B, CS00);
	setBitHigh(&TCCR0B, CS01);
	setBitLow(&TCCR0B, CS02);

	//PWM-frequency calculations:
	//
	//Timer0Frequency = ATmegaFrequency * Prescaler = 16 000 000 / 64 = 250 000 Hz
	//Bottom=0, TOP = 255. A PWMCycle is 256 counts. (We start at 0).
	//PWM-frequency = 250 000 / 256 = 976.5625 hz
	//Almost 1000 pwmcykles/second.
}

void timer2Init(){
	//set CTC (Clear Timer on Compare Match Mode)
	setBitLow(&TCCR2A, WGM20);
	setBitHigh(&TCCR2A, WGM21);
	setBitLow(&TCCR2B, WGM22);

	//set Prescaler 1024
	setBitHigh(&TCCR2B, CS20);
	setBitHigh(&TCCR2B, CS21);
	setBitHigh(&TCCR2B, CS22);

	//set OCR2A value. 

	OCR2A = 249; //Match flag OCF2A is set when Timer matches OCR2A.

	//OCR2A Calculations:
	//
	//TimerFrequency = ATmegaFrekvens / Prescaler = 16 000 000 / 1024 = 15625 Hz
	//TimerPeriod = 16 ms = 0.016 s
	//OCR2A = (15625 * 0.016)-1 = 250
	//Substract 1 because we start counting at 0.
}
	


