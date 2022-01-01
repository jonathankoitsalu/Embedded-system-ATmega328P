#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>

#include "led.h"
#include "serial.h"
#include "timer.h"
#include "adc.h"
#include "bitshift.h"
#include "button.h"

//deluppgift 4: ANVÄNDARSTYRT BETEENDE
//
//timer1 konfigureras till pwm för att kontrollera blått ljus (digital pin 9) på arduinoskölden
//timer2 konfigureras till CTC med 100 hz alltså 10ms period. Den är interruptstyrd.
//digital pin 8 interuppt aktiveras för att få tillgång till sköldens key1 knapp.
//
//stateExecuter körs varje gång timer2 tickar eftersom alla states (förutom ledoff) är timerberoende.
//stateHandler byter state vid knappinterrupt och skriver ut nuvarande state på serial.

//timer2 variables
volatile bool timerDebounce = false;
volatile bool timerStateExecuter = false;

volatile bool debounceTimer;
volatile bool buttonInterrupt = false;

bool released; //buttonReader variable
bool releasedConfirm; //debounce variable. activates statechange.

int state = 1;

volatile uint8_t adcValue = 0;

void main (void) {
	timerInit();
	uart_init();

	buttonInit();
	ledInit();
	adcInit();

	sei();

	while(1){
	//Alla dessa funktioner är nedanför i main.c filen
	buttonReader(); 
	debounce();
	stateHandler();
	stateExecuter();
	}
}

ISR(TIMER2_COMPA_vect) {
	timerDebounce = true;
	timerStateExecuter = true;
}

ISR(ADC_vect) {
	/*
		ISR interrupt that triggers when ADC conversation is finished
		Adc converted value is set to adcValue variabel
	*/
	adcValue = ADCH;
}

ISR(PCINT0_vect) {
	if(!timerDebounce)buttonInterrupt = true; //Only read interrupts while debouncetimer is off
}

void buttonReader(){
	if(buttonInterrupt){
		if(!(PINB & (1 << PB0))) {released = true;} //reads if PB0 is 0, meaning button is released.
		buttonInterrupt = false; //reseting interrupt bool
		TCNT2=0; //restart timer
	}
}

void debounce(){
	if(timerDebounce){//enters this loop 10 ms after timer restarts. 
		if(released){
			if(!(PINB & (1 << PB0))){releasedConfirm=true;} //checks if button is still released after 10 ms.
			released = false;
			}
		timerDebounce = false;
	}
}

void stateHandler(){
	if(releasedConfirm){ //ny state om knappen har släppts upp
		releasedConfirm=false;
		switch(state){
			case 1:
				state++;
				printf_P(PSTR("2\r\n"));
				break;
			case 2:
				state++;
				printf_P(PSTR("3\r\n"));
				break;
			case 3:
				state++;
				printf_P(PSTR("4\r\n"));
				break;
			case 4:
				state=1;
				printf_P(PSTR("1\r\n"));
				break;
		}
	}
}

void stateExecuter(){
	if(timerStateExecuter){ //kör stateExecuter var 10ms alltså varje gång timer2 interruptas.
		timerStateExecuter=false;
		switch(state){
			case 1:
				pulse(); //funktion finns i led.c
				break;
			case 2:
				potentiometer(); //funktion finns nedan.
				break;
			case 3:
				blink(); //funktion finns i led.c
				break;
			case 4:
				off(); //funktion finns i led.c
				break;
		}
	}
}

void potentiometer(){
	ADCSRA |= (1 << ADSC); //starts ADC conversion
	OCR1A = adcValue;
}

