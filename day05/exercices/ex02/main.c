#include "main.h"

int main()
{

	char	to_write[] = "I need this!";
	char	to_write1[] = "I need this!";
	char	read_buffer[1024] = {0};
	size_t	offset_start_0 = 0x1;
	size_t	offset_start_1 = offset_start_0 + 20;

	_delay_ms(1000);

	init_USART();

	uart_print_str("====\r\nSTART\r\n====\r\n\n");

	eeprom_write_byte((void *)7, 1);
	eeprom_write_byte((void *)offset_start_0, 1);
	eeprom_write_byte((void *)offset_start_1, 1);

	// uart_print_str("Memory reset start\r\n");
	// reset_all_memory();
	// uart_print_str("Memory reset done\r\n");
	// while (1);

	if (1)
	{
		uart_print_str("\r\nwrite str: \r\n");
		if (safe_eeprom_write(to_write, offset_start_0, 13))
		{
			uart_print_str("  ERROR\r\n");
		}
		else 
		{
			uart_print_str("  SUCCESS\r\n");
		}
		_delay_ms(1000);
	}
	if (1)
	{
		uart_print_str("\r\nRead written str: \r\n");
		if(safe_eeprom_read(read_buffer, offset_start_0, 13))
		{
			uart_print_str(" ERROR\r\n");
		}
		else 
		{
			uart_print_str(" SUCCESS: \"");
			uart_print_str(read_buffer);
			uart_print_str("\"\r\n");
		}
		_delay_ms(1000);
	}

	if (1)
	{
		uart_print_str("\r\ninvalid second write:\r\n");
		if (safe_eeprom_write(to_write1, offset_start_1 + 10, 3))
		{
			uart_print_str("SUCCESS\r\n");
		}
		else
		{
			uart_print_str("ERROR: second write success\r\n");
			_delay_ms(1000);
		}
	}

	if (1)
	{
		uart_print_str("\r\nValid third write :\r\n");
		if (safe_eeprom_write(to_write1, offset_start_1 + 20, 3))
			uart_print_str("ERROR: third write failed\r\n");
		else
			uart_print_str("SUCCESS: write third wrote\r\n");
		_delay_ms(1000);
	}

	while (1);

	eeprom_write_byte((void *)offset_start_0, 1);
	eeprom_write_byte((void *)offset_start_1, 1);
	return (0);
}

void	reset_all_memory(void)
{
	uint8_t	reset[1024] = {0};

	eeprom_write_block(reset, 0, 1023);
}


//... all ...

static uint16_t	_get_memory_length(void *memory_header)
{
	return (*((uint16_t*)(memory_header + 1)));
}


//... read ...

bool safe_eeprom_read(void *buffer, size_t offset, size_t length)
{
	uint8_t header[MEMORY_HEADER_LENGTH];

	// uart_print_str("safe_eeprom_read: \r\n");
	if (buffer == 0 || length < 0 || offset < 0
			|| EEPROM_ADDRESS_MAX - (length + 3)< offset)
		return (1); 
	// uart_print_str("  reading...\r\n");
	eeprom_read_block(header, (void *)offset, MEMORY_HEADER_LENGTH);
	// uart_print_str("  magic: ");
	// uart_print_hexa(header[0] >> 1);
	// uart_print_str("  length: ");
	// uart_print_hexa(header[1]);
	// uart_print_str("\r\n");
	if(header[0] >> 1 != MAGIC_NUMBER
		|| (header[0] ) == MEMORY_FREE
		|| _get_memory_length(header) < length)
		return (1);
	// uart_print_str("read the block...\r\n");
	eeprom_read_block(buffer, (void *)offset + MEMORY_HEADER_LENGTH, length);
	// uart_print_str("read the block...\r\n");
	return (0);
}


// ... write ...

static bool	_is_in_allocated_memory(size_t offset)
{
	uint8_t 		check_magic;
	uint8_t			header[MEMORY_HEADER_LENGTH];
	memory_state	allocated;
	uint8_t			*poffset;
	uint8_t			*i;

		// uart_print_str("    _is_in_allocated_memory:\r\n");
	if (offset <= 0)
		return false;
	allocated = false;
	poffset = (uint8_t *)offset;
	i= 0;
	while (i < poffset)
	{
			// uart_print_str("     - ");
			// uart_print_hexa((unsigned int) i);
			// uart_print_str(": ");
		check_magic = eeprom_read_byte(i);
			// uart_print_hexa(check_magic);
			// uart_print_str("\r\n");
		if ((check_magic >> 1) == MAGIC_NUMBER)						//find memory previously allocated//
		{
				// uart_print_str("      ");
				// uart_print_hexa((int)i);
				// uart_print_str(" is magic_number\r\n");
				// _delay_ms(3000);
			allocated = (check_magic & 1);
			if ((size_t)i > EEPROM_ADDRESS_MAX - MEMORY_HEADER_LENGTH)
				break;
			eeprom_read_block(header, i, MEMORY_HEADER_LENGTH);
				// uart_print_str("      jump to : ");
				// uart_print_hexa((int) i + _get_memory_length(header));
				// uart_print_str("\r\n");
			i += MEMORY_HEADER_LENGTH + _get_memory_length(header);
			if (i >= poffset)
				return (true);
			continue;
		}
		++i;
	}
	return (false);
}

static bool	_has_enough_free_space(size_t offset, size_t length)
{
	uint8_t		*poffset;
	uint8_t 	check_magic;
	uint8_t		header[MEMORY_HEADER_LENGTH];
	uint16_t	i;

	// uart_print_str("  _has_enough_free_space: \r\n");
	poffset = (void *)offset;
	i = 0;
	while (i < length && i + offset <= EEPROM_ADDRESS_MAX)
	{
		// uart_print_hexa(i);
		// uart_print_str(" :");
		check_magic = eeprom_read_byte(poffset + i);
		// uart_print_hexa(check_magic);
		if(check_magic >> 1 == MAGIC_NUMBER)
		{
			if (offset + i >= EEPROM_ADDRESS_MAX - MEMORY_HEADER_LENGTH) //if magic_number too close to the max memory to write a memory header, considered as free (can only happen at init)
				continue;
			if ((check_magic & 1) == MEMORY_ALLOCATED)
				return (false);
			else
			{
				eeprom_read_block( &header, poffset + i, MEMORY_HEADER_LENGTH);
				i += MEMORY_HEADER_LENGTH + _get_memory_length(header); //jumps the previously allocated memory, -1for further ++
				continue;
			}
		}
		// uart_print_str("\r\n");
		++i;
	}
	if ( i < length)
		return (false);
	return (true);
}

static bool	_can_be_allocated(size_t offset, size_t length)
{
	uint8_t 	check_magic;
	uint16_t	block_length;
	uint16_t	i;

	if (length < 0 || offset < 0
			|| EEPROM_ADDRESS_MAX - (length + MEMORY_HEADER_LENGTH)< offset
			|| _is_in_allocated_memory(offset)
			|| ! _has_enough_free_space(offset, length))
		return (false); 
	return (true);
}

static bool	_is_already_written(uint8_t * to_write, size_t offset, size_t length)
{
	uint8_t	header[MEMORY_HEADER_LENGTH];
	uint8_t	*poffset;


	poffset = (uint8_t *)offset;
	if (offset + MEMORY_HEADER_LENGTH > EEPROM_ADDRESS_MAX)
		return (false);
	eeprom_read_block(header, (uint8_t *)offset, MEMORY_HEADER_LENGTH);
	if ( header[0] >> 1 != MAGIC_NUMBER
		||	_get_memory_length(header) != length)
		return (false);

	size_t	i;
	for ( i = 0; i < length && offset + MEMORY_HEADER_LENGTH + i < EEPROM_ADDRESS_MAX; ++i)
	{
		if (to_write[offset + MEMORY_HEADER_LENGTH + i] != eeprom_read_byte(poffset + EEPROM_ADDRESS_MAX + i))
			return (false);
	}
	return (true);
}

static void _write_with_metadata(uint8_t * buffer, size_t offset, size_t length)
{
	uint8_t	*poffset;

	poffset = (uint8_t *)offset;
		// uart_print_str("  _write_with_metadata\r\noffset: ");
		// uart_print_hexa(offset);
	eeprom_write_byte(poffset, MAGIC_NUMBER * 2 + MEMORY_ALLOCATED);
		// uart_print_str(" , write magic: ");
		// uart_print_hexa(eeprom_read_byte(poffset));
		// uart_print_str("  _write_with_metadata1\r\n");
	eeprom_write_word((uint16_t *)(poffset + 1), (uint16_t)length);
		// uart_print_str("  _write_with_metadata2\r\n");
	eeprom_write_block(buffer, poffset + MEMORY_HEADER_LENGTH, length);
	// uart_print_str("  _write_with_metadata3\r\n");
}

bool safe_eeprom_write(void * buffer, size_t offset, size_t length)
{

		// uart_print_str("\r\n_is_already_written : ");
		// uart_print_hexa(_is_already_written(buffer,offset, length));

		// uart_print_str("\r\n\r\n_is_in_allocated_memory : \r\n");
		// uart_print_hexa(_is_in_allocated_memory(offset));
		// uart_print_str("\r\n\r\n_has_enough_free_space : \r\n");
		// uart_print_hexa(_has_enough_free_space(offset, length));
		// uart_print_str("\r\n\r\n_can_be_allocated : \r\n");
		// uart_print_hexa(_can_be_allocated(offset, length));
		// uart_print_str("\r\n");


	if (_is_already_written(buffer,offset, length))
		return (0);
	if (!_can_be_allocated(offset, length))
		return (1);
	_write_with_metadata(buffer, offset,length);
	return (0);
}

