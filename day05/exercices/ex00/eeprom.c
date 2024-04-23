#include "eeprom.h"

eeprom_status	eeprom_write(uint16_t address, uint8_t data)
{
	if (address > EEPROM_ADDRESS_MAX)
		return (EEPROM_BAD_ADDRESS);
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address and Data Registers */
	EEARH =  (address >> 8) & 3;
	EEARL= address & 0xFF;
	EEDR = data;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
	uint8_t	data_check;
	eeprom_read(address, &data_check);
	if (data != data_check)
		return (EEPROM_WRITE_FAIL);
	return (EEPROM_OK);
}

eeprom_status	eeprom_write_batch(uint16_t address, void * data, uint16_t size)
{
	uint16_t		i;
	eeprom_status	status;

	if ((uint32_t)(address + size) > EEPROM_ADDRESS_MAX)
		return (EEPROM_BAD_ADDRESS);
	uint8_t		*data8 = (uint8_t *)data;
	status = EEPROM_OK;
	for (i = 0; i < size; ++i)
	{
		status = eeprom_write(address + i, data8[i]);
		if ( status != EEPROM_OK)
			return (status);
	}
	return (EEPROM_OK);
}

eeprom_status eeprom_read(uint16_t address, uint8_t *data)
{
	if (address > EEPROM_ADDRESS_MAX)
		return (EEPROM_BAD_ADDRESS);
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address register */
	EEARH =  (address >> 8) & 3;
	EEARL= address & 0xFF;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	*data = EEDR;
	return (EEPROM_OK);
}

eeprom_status	eeprom_read_batch(uint16_t address, void *data, uint16_t size)
{
	uint16_t	i;

	if ((uint32_t)(address + size) > EEPROM_ADDRESS_MAX)
		return (EEPROM_BAD_ADDRESS);
	for (i = 0; i < size; ++i)
		eeprom_read(address + i, data + i);
	return (EEPROM_OK);
}