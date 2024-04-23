#ifndef MAIN_H
# define MAIN_H

# include "tools.h"
# include "eeprom.h"
# include "UART.h"

# define MAGIC_NUMBER 0x03 // <  (256 >> 1) = 128
# define MEMORY_HEADER_LENGTH 3 // magic(1) + length(2)

typedef enum
{
	MEMORY_FREE,
	MEMORY_ALLOCATED,
}	memory_state;

bool 	safe_eeprom_write(void * buffer, size_t offset, size_t length);
bool 	safe_eeprom_read(void *buffer, size_t offset, size_t length);
void	reset_all_memory(void);

#endif