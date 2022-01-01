#include <avr/io.h>
#include <util/delay.h>
#include "stdint.h"

#include "serial.h"
#include "led.h"

void main (void) {
    char buffer[50];
    pinDirection(&DDRB, OUTPUT, PB1); //direction output
    setBitHigh(&PORTB, PB1); //blue led off
    uartInit(); //initiate UART
    //blinkLed();
    while(1){
    //printChar('A'); //prints a char
    //printString("Jonathan Koitsalu\r"); //prints a char array
    //print(); //prints chars sent with keyboard

    //storeCommand1(buffer); // adapted for all monitors.
    //executeCommand1(buffer);

    storeCommand2(buffer); // adapted for monitors that send both newline and carriage return when you press enter.
    executeCommand2(buffer);
    }
}






