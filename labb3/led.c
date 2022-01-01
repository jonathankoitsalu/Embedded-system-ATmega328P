#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>

#include "serial.h"
#include "led.h"
#include "bitshift.h"

uint8_t pwm = 0;
uint8_t direction;
int count = 0;
bool toggle;

void ledInit() {
    pinDirection(&DDRB, OUTPUT, PB1);
}

uint8_t simpleRamp(){
    if(pwm == 0) direction=1;
    if(pwm == 255) direction=-1;
    pwm+=direction;
    return pwm;
}

void pulse(){
    OCR1A = simpleRamp(pwm, direction); //updates timer0 duty cycle with simpleramp.
}

void blink(){
    count++;
    if(count==50){
        toggle=!toggle;
        if(toggle)OCR1A = 0;
        else OCR1A = 255;
        count=0;
    }
}

void off(){
    OCR1A = 255;
}

