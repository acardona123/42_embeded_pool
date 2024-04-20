#ifndef MAIN_H
# define MAIN_H

# include "tools.h"
# include "UART.h"
# include "I2C.h"

# define THERMO_SENSOR_ADDRESS 0x38
# define THERMO_SENSOR_INIT_TIME 40 //ms

# define THERMO_SENSOR_CMD_INIT 0b10111110
# define THERMO_SENSOR_CMD_TRIGGER_MEASURE 0b10101100
# define THERMO_SENSOR_CMD_sensor_temp_soft_reset 0b10111010

# define THERMO_SENSOR_BIT_CALIBRATION_ENABLED 3

# define THERMO_SENSOR_NUMBER_OF_BYTE_PER_MEASURE 7
# define THERMO_SENSOR_CHECK_CRC 0

# define TWO_POWER_TWENTY 1048576.f



e_success	sensor_temp_soft_reset( void );
void		sensor_temp_read_in_loop( void );


#endif