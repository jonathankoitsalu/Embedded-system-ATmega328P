#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdbool.h>

void uartInit();
bool monitorWritable();
void printChar(unsigned char data);
void printCharCR(unsigned char data);
void printString(char* str);
unsigned char readChar();
void print();
void storeCommand1(char* buffer);
void executeCommand1(char* buffer);
void storeCommand2(char* buffer);
void executeCommand2(char* buffer);

#endif

