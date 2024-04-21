#ifndef AHT20_H
# define AHT20_H

# include "UART.h"
# include "I2C.h"

# define AHT20_ADDRESS 0x38

# define AHT20_INIT_TIME 40 //ms
# define AHT20_CONNECTION_TIME 40 //ms

# define AHT20_CMD_GET_STATUS  0x71
# define AHT20_CALIBRATION_NEEDED (1 << 3)
# define AHT20_CALIBRATE 0xBE
# define AHT20_CALIBRATION_DELAY 10 //ms

# define AHT20_MEASURE	0xAC
# define AHT20_MEASURE_DELAY 80 //ms
# define AHT20_MEASURE_SECURITY_DELAY 20 //ms
# define AHT20_MEASURE_ONGOING_MASK (1 << 7)

# define TWO_POWER_TWENTY 1048576.f

int8_t	aht20_init(void);
int8_t	aht20_measure(float *temperature, float *humidity);
int8_t	aht20_averaged_measure(float *temperature, float *humidity);

#endif