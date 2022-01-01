#include <avr/io.h>
#include "adc.h"
#include "led.h"
#include "bitshift.h"

void adcInit(){

    //pin A0 set to input.
    pinDirection(&DDRC, INPUT, PC0);

    //AVCC with external capacitor at AREF pin
    setBitHigh(&ADMUX, REFS0);
    setBitLow(&ADMUX, REFS1);

    //Prescaler 8
    setBitHigh(&ADCSRA, ADPS0);
    setBitHigh(&ADCSRA, ADPS1);
    setBitLow(&ADCSRA, ADPS2);

    //ADC Left Adjust Result
    setBitHigh(&ADMUX, ADLAR);

    //ADC Enable
    setBitHigh(&ADCSRA, ADEN);

    //ADC Interrupt Enable
    setBitHigh(&ADCSRA, ADIE);
}




