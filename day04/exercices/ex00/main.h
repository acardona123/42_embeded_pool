#ifndef MAIN_H
# define MAIN_H

# include "tools.h"
# include "UART.h"
# include "I2C.h"

# define THERMO_SENSOR_ADDRESS 0x38
# define THERMO_SENSOR_INIT_TIME 20 //ms

void	i2c_print_status( void );
void	i2c_start( void );//start+_i2c_send_SLA
void	i2c_stop( void );

#endif