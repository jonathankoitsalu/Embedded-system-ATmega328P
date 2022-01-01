#include <avr/io.h>

#include "timer.h"


void timerInit(){
	timer1Init();
	timer2Init();
}

void timer1Init(){
	//set fast PWM mode
	setBitHigh(&TCCR1A, WGM10);
	setBitLow(&TCCR1A, WGM11);
	setBitHigh(&TCCR1B, WGM12);
	setBitLow(&TCCR1B, WGM13);

	//set non-inverting mode
	setBitLow(&TCCR1A, COM1A0);
	setBitHigh(&TCCR1A, COM1A1);

	//set Prescaler 64
	setBitHigh(&TCCR1B, CS10);
	setBitHigh(&TCCR1B, CS11);
	setBitLow(&TCCR1B, CS12);

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

	//enabling output compare a match interrupt
	setBitHigh(&TIMSK2, OCIE2A); 

	//set OCR2A value. 100Hz.

	OCR2A = 155;
}

