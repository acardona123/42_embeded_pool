#ifndef I2C_H
# define I2C_H

# include "tools.h"

#include <util/twi.h>

#define I2C_BIT_RATE_FREQUENCY 100000//Hz

typedef enum e_mode_RW
{
	MODE_READ,
	MODE_WRITE
}	e_mode_RW;

void	_i2c_init( void );
int8_t	_i2c_start( void );
int8_t	_i2c_stop( void );
int8_t	_i2c_start( void );
int8_t	_i2c_repeated_start();
int8_t	_i2c_send_SLA( char slave_addr, e_mode_RW mode);
int8_t	_i2c_sending_data_char( char data );
int8_t _i2c_sending_data_str( char *data, long unsigned  len );
char	_i2c_receiving_data( bool stop_after_reception  );


#endif