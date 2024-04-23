#ifndef EEPROM_H
# define EEPROM_H

# ifndef __AVR_ATmega328P__
#  define __AVR_ATmega328P__ 1
# endif

# include <avr/io.h>
# include <avr/eeprom.h>

# define EEPROM_ADDRESS_MAX 1023

typedef enum
{
	EEPROM_OK,
	EEPROM_BAD_ADDRESS,
	EEPROM_WRITE_FAIL,
}	eeprom_status;

eeprom_status	eeprom_write(uint16_t address, uint8_t data);
eeprom_status	eeprom_read(uint16_t address, uint8_t *data);
eeprom_status	eeprom_write_batch(uint16_t address, void * data, uint16_t size);
eeprom_status	eeprom_read_batch(uint16_t address, void *data, uint16_t size);


#endif