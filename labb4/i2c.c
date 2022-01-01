#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <string.h>

#include "i2c.h"

void i2c_init(void) {
	

	TWCR |= (1<<TWEN);
	TWSR = 0x00; // prescaler 1

	// sclFreq = cpuFreq / (16 + (2*twbr*prescaler))
	// sclFrq = 100 000 Hz
	// cpuFreq = 16 000 000 Hz
	// prescaler = 1
	// twbr = ((cpuFreq / sclFreq) - 16)/(2*prescaler) = 72 = 0x48

	TWBR = 0x48;
}

void i2c_meaningful_status(uint8_t status) {
	switch (status) {
		case 0x08: // START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("START\n"));
			break;
		case 0x10: // repeated START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("RESTART\n"));
			break;
		case 0x38: // NAK or DATA ARBITRATION LOST
			printf_P(PSTR("NOARB/NAK\n"));
			break;
		// MASTER TRANSMIT
		case 0x18: // SLA+W transmitted, ACK received
			printf_P(PSTR("MT SLA+W, ACK\n"));
			break;
		case 0x20: // SLA+W transmitted, NAK received
			printf_P(PSTR("MT SLA+W, NAK\n"));
				break;
		case 0x28: // DATA transmitted, ACK received
			printf_P(PSTR("MT DATA+W, ACK\n"));
			break;
		case 0x30: // DATA transmitted, NAK received
			printf_P(PSTR("MT DATA+W, NAK\n"));
			break;
		// MASTER RECEIVE
		case 0x40: // SLA+R transmitted, ACK received
			printf_P(PSTR("MR SLA+R, ACK\n"));
			break;
		case 0x48: // SLA+R transmitted, NAK received
			printf_P(PSTR("MR SLA+R, NAK\n"));
			break;
		case 0x50: // DATA received, ACK sent
			printf_P(PSTR("MR DATA+R, ACK\n"));
			break;
		case 0x58: // DATA received, NAK sent
			printf_P(PSTR("MR DATA+R, NAK\n"));
			break;
		default:
			printf_P(PSTR("N/A %02X\n"), status);
			break;
	}
}

inline void i2c_start() {
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); //TWSTA set to become a master. TWINT and TWEN starts twi operation.
	while (!(TWCR & (1<<TWINT))); //Wait that the current twi process is complete. 
}

inline void i2c_stop() {
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN); //Transmit STOP condition.
	while(TWCR&(1<<TWSTO)); // TWSTO clears when STOP condition is executed.
}

inline uint8_t i2c_get_status(void) {
	uint8_t status;
	status = TWSR & 0xF8; //the status of the TWI logic and the 2-wire serial bus.

	return status;
}

inline void i2c_xmit_addr(uint8_t address, uint8_t rw) {
	TWDR = (address & 0xfe) | (rw & 0x01); //write address and read or write into byte to be transmitted.
	TWCR = (1 << TWINT) | (1 << TWEN); //start the twi operation.
	while (!(TWCR & (1 << TWINT))); //Wait that the current twi process is complete. 
}

inline void i2c_xmit_byte(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN); //start the twi operation.
	while (!(TWCR & (1 << TWINT))); //Wait that the current twi process is complete. 
}

inline uint8_t i2c_read_ACK() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); //start the twi operation. TWEA generates a ACK pulse.
	while (!(TWCR & (1 << TWINT))); //Wait that the current twi process is complete. 
	return TWDR; 
}

inline uint8_t i2c_read_NAK() {
	TWCR = (1 << TWINT) | (1 << TWEN); //start the twi operation.
	while (!(TWCR & (1 << TWINT))); //Wait that the current twi process is complete. 
	return TWDR;
}

inline void eeprom_wait_until_write_complete() { //eeprom returns ack when writing is complete. 
	while (i2c_get_status() != 0x18) {
		i2c_start();
		i2c_xmit_addr(ADDR, I2C_W);
	}
}

uint8_t eeprom_read_byte(uint8_t eeprom_adrr) {
	uint8_t readByte;
	
	i2c_start(); //start
	i2c_xmit_addr(ADDR, I2C_W); //tell eeprom to get ready to move pointer.
	i2c_xmit_byte(eeprom_adrr); //move pointer to this adress.

	i2c_start(); //restart
	i2c_xmit_addr(ADDR, I2C_R); //read adress.
	readByte = i2c_read_NAK(); //read byte and nack.

	i2c_stop(); //stop

	return readByte;
}

void eeprom_write_byte(uint8_t eeprom_adrr, uint8_t *data) {

	i2c_start(); //start
	i2c_xmit_addr(ADDR, I2C_W); //tell eeprom to get ready to move pointer.

	i2c_xmit_byte(eeprom_adrr); //move pointer to this adress.
	i2c_xmit_byte(data); //write data on this adress.


	i2c_stop(); //stop
	eeprom_wait_until_write_complete(); //wait for eeprom to complete task.
}



void eeprom_write_page(uint8_t addr, uint8_t *data) {

	i2c_start(); //start
	i2c_xmit_addr(ADDR, I2C_W); 
	i2c_xmit_byte(addr); //move eeprom pointer to this adress.

	for (int i = 0; i < 8; i++) { 
		i2c_xmit_byte(data[i]);
	}
	
	i2c_stop();
	eeprom_wait_until_write_complete();
}

void eeprom_sequential_read(char *readEeprom, uint8_t start_addr, uint8_t len) {

	i2c_start(); //start
	i2c_xmit_addr(ADDR, I2C_W); 
	i2c_xmit_byte(start_addr);

	i2c_start();
	i2c_xmit_addr(ADDR, I2C_R);
	
	for (int i = 0; i < (len - 1); i++) {
		readEeprom[i] = i2c_read_ACK(); //ACK means still reading
	}	
	readEeprom[len - 1] = i2c_read_NAK(); //NACK means finnished reading

	i2c_stop();
}

