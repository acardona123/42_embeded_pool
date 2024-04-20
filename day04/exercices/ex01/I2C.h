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

void		_i2c_init( void );
void		_i2c_stop( void );
e_success	_i2c_start( void );
e_success	_i2c_start( void );
e_success	_i2c_repeated_start( void );
e_success	_i2c_send_SLA( char slave_addr, e_mode_RW mode);
e_success	_i2c_sending_data_char(char data);
e_success	_i2c_sending_data_str(char *data);
char		_i2c_receiving_data( bool stop_after_reception  );
e_mode_RW	_i2c_get_RW_status( void );

#endif