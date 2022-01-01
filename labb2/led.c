#include <avr/io.h>
#include "stdint.h"
#include "led.h"
#include "bitShift.h"

void ledInit() {
	pinDirection(&DDRD, OUTPUT, PD6); //direction output.
}

void fade(){
    uint8_t pwm = 0;
	uint8_t direction;
    while(1){
	if(TIFR2 & (1<<OCF2A)){//Check timer2 for match.
			setBitHigh(&TIFR2, OCF2A); //clears the match flag.
			OCR0A = simpleRamp(&pwm, &direction); //updates timer0 duty cycle with simpleramp.
		}
	}
}

uint8_t simpleRamp(uint8_t *pwm, uint8_t *direction){
    if(*pwm == 0) *direction=1;
    if(*pwm == 255) *direction=-1;
    *pwm+=*direction;
    return *pwm;
}