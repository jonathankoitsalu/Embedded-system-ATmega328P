#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "i2c.h"
#include "serial.h"

// del 3: UTÖKAD FUNKTIONALITET  

// eeprom_write_page() testas med 4 stycken 8-teckens stringar som skrivs in i varsitt "page". Minnesadressen inkrementeras med 8 steg för varje page.
// eeprom_sequential_read() testas genom att skriva ut dessa 4 page:s som skrivs in.
// eeprom_sequential_write() denna funktion delar upp data i pages (8-teckens strängar) vilka sparas i tur och ordning i eepromet med eeprom_write_page(). Om sista bytes:en inte fyller ut en page så sparas dom en byte i taget med eeprom_write_byte.
// eeprom_sequential_write() är i main.c.
void main (void) {
	i2c_init();
	uart_init();
	sei(); // Interrupts enabled

	char page1[8] = "Detta   ";
	char page2[8] = "kommer  ";
	char page3[8] = "från    ";
	char page4[8] = "eepromen";

	char readEeprom[40] = "";

	
	while (1) {

	// Fråga 2:
	// Skriv några pages med data i EEPROMet vid uppstart, och läs sedan ut samtligt data i en operation och 
	// skriv ut en hexdump (samt ”char:s” om det är text du skrivit in) av det på UARTen.
	
	eeprom_write_page(EEPROM_ADDR, page1);
	eeprom_write_page(EEPROM_ADDR+8, page2);
	eeprom_write_page(EEPROM_ADDR+16, page3);
	eeprom_write_page(EEPROM_ADDR+24, page4);

	eeprom_sequential_read(readEeprom, EEPROM_ADDR , 32);

	printf_P(PSTR("testar eeprom_write_page och eeprom_sequential_read: \n"));
	printReadEeprom(readEeprom);

	// Fråga 3:
	// Skriv hela eller delar av EEPROMet sekvensiellt. 

	eeprom_sequential_write(EEPROM_ADDR, "Denna mening kommer från eepromen");
	eeprom_sequential_read(readEeprom, EEPROM_ADDR, strlen("Denna mening kommer från eepromen"));
	printf_P(PSTR("testar eeprom_sequential_write: \n"));
	printReadEeprom(readEeprom);
	_delay_ms(10000);
	}
	
}

void printReadEeprom(char* readEeprom){
	printf_P(PSTR("char: %s | hex: "), readEeprom);
	for (int i = 0; i < strlen(readEeprom); i++) {
	printf_P(PSTR("%x "), readEeprom[i]);
	}
	printf_P(PSTR("\n"));
}

void eeprom_sequential_write(uint8_t addr, uint8_t *data){
	uint8_t pageData[8];
	int page;
	int pageStartIndex;
	int pageEndIndex;
	int adressIncrement;
	int i;
	int pages = (strlen(data)/8); //antal pages i datasträngen
	for(page = 0; page < pages; page++){ //delar upp datat i pages
		pageStartIndex = page*8;
		adressIncrement = pageStartIndex;
		pageEndIndex = pageStartIndex+8;
		for (i = pageStartIndex; i < pageEndIndex; i++){ //läser in varje page i pageData
			pageData[i-page*8] = data[i];
		}
		eeprom_write_page(addr+adressIncrement, pageData); //Skriver varje page på en ny adress.
	}
	i = 0;
	int lastBytes=strlen(data)%8;
	while(lastBytes--){ //Om det finns någon återstående byte att skriva in så körs denna.
		eeprom_write_byte((addr+pages*8)+i, data[(pages*8)+i]); //skriv in varje återstående byte på en egen adress.
		i++;
	}
}


