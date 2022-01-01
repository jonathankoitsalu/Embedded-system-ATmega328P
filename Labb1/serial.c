#include <avr/io.h>
#include <util/delay.h>

#include "stdint.h"
#include "serial.h"

void uartInit()
{
unsigned long BAUD = 38400UL;
unsigned long UBRR = ((F_CPU/(16*BAUD))-1);

//Set baud rate                    
UBRR0H = (UBRR & 0xFF00)>>8; //insert the most significant bits.
UBRR0L = UBRR & 0x00FF; //insert the least significant bits.

//Enable receiver and transmitter
setBitHigh(&UCSR0B,RXEN0);
setBitHigh(&UCSR0B,TXEN0);

//sets type of parity generation and check. Set to no parity.
setBitLow(&UCSR0C,UPM01);
setBitLow(&UCSR0C,UPM00);

//sets the number of stop bits to be inserted by the transmitter. Set to 1 stopbit.
setBitLow(&UCSR0C,USBS0);

//sets the number of data bits in a frame the receiver and transmitter use. Set to 8bit.
setBitLow(&UCSR0C,UCSZ02); 
setBitHigh(&UCSR0C,UCSZ01);
setBitHigh(&UCSR0B,UCSZ00);
}

bool monitorWritable() //checks if monitor is writable.
{
    if (UCSR0A & (1<<UDRE0)){return true;} //Checks that the transmitbuffer is empty and writable.
    else {return false;}
}

void printChar(unsigned char data)//Prints a char.
{
    if (monitorWritable()){
        UDR0 = data;}   //load transmitbuffer with data.
}

void printCharCR(unsigned char data){ //Prints a char. If char is '\r' then first print '\n' then print char (which is '\r').
    if(data=='\r'){
        printChar('\n');
    }
    printChar(data);
}

void printString(char* str){ //Prints a char array.
    int i = 0;
    while(str[i]!='\0'){
        printChar(str[i]);
        i++;
    }
}

unsigned char readChar(){   //Reads char.
    while (!(UCSR0A & (1<<RXC0))); //Checks if there is unread data in the receive buffer.
    return UDR0; //return char
}

void print() //Prints the read char.
{
    printChar(readChar());
}

void storeCommand1(char* buffer) //Stores chars in a buffer until carriage return or newline is read. Carriage return and newline are not stored in the buffer.
{
    int j = 0;
    char lastChar;
    lastChar = readChar();
    printChar(lastChar);
    while((lastChar != '\r' || lastChar != '\n') && j<49){
        buffer[j]=lastChar;
        j++;
        lastChar = readChar();
        printChar(lastChar);
    }
    buffer[j]='\0';
}

void executeCommand1(char* buffer) //Compares the buffer with commands to be executed.
{
    if(!strcmp(buffer,"on")){ //turns pin9 led on.
        setBitLow(&PORTB, PB1);
    }
    if(!strcmp(buffer,"off")){  //turns pin9 led off.
        setBitHigh(&PORTB, PB1);
    }
}

void storeCommand2(char* buffer) //Stores chars in a buffer until newline is read. Stores both carriage return and newline in the buffer.
{
    int j = 0;
    char lastChar;
    while(lastChar != '\n' && j<49){
        lastChar = readChar();
        printChar(lastChar);
        buffer[j]=lastChar;
        j++;
    }
    buffer[j]='\0';
}

void executeCommand2(char* buffer) //Compares the buffer with commands to be executed.
{
    if(!strcmp(buffer,"on\r\n")){ //turns pin9 led on.
        setBitLow(&PORTB, PB1);
    }
    if(!strcmp(buffer,"off\r\n")){  //turns pin9 led off.
        setBitHigh(&PORTB, PB1);
    }
}

