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
void	_i2c_start( void );
void	_i2c_stop( void );
void	_i2c_start( void );
void	_i2c_repeated_start();
void	_i2c_send_SLA( char slave_addr, e_mode_RW mode);
void	_i2c_sending_data_char(char data);
void	_i2c_sending_data_str(char *data);
char	_i2c_receiving_data( bool stop_after_reception  );


#endif