#ifndef I2C_H
# define I2C_H

# include "tools.h"

#include <util/twi.h>

#define I2C_BIT_RATE_FREQUENCY 100000//Hz

# define NACK 0
# define ACK 1

typedef enum e_mode_RW
{
	MODE_READ,
	MODE_WRITE
}	e_mode_RW;

void 	_i2c_init( void );
int8_t	i2c_start(uint8_t addr, e_mode_RW RW_mode);
void	i2c_stop(void);
int8_t	i2c_write(unsigned char data);
uint8_t	i2c_read(uint8_t ack, uint8_t *dest);



#endif