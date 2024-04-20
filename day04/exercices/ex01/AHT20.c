#include "AHT20.h"


// .... Init ....

static int8_t	_aht_calibration(void);

int8_t	aht20_init(void)
{
	uint8_t aht_status;

	//wait for the sensor to start
	_delay_ms(AHT20_INIT_TIME);

	//get the sensor status
	if (i2c_start(AHT20_ADDRESS, MODE_WRITE))
		return (1);
	if (i2c_write(AHT20_CMD_GET_STATUS))
	{
		i2c_stop();
		return (1);
	}
	i2c_stop();
	if ( i2c_start(AHT20_ADDRESS, MODE_READ))
		return (1);
	if ( i2c_read(NACK, &aht_status))
	{
		i2c_stop();
		return (1);
	}
	i2c_stop();

	// do calibration fs needed with status
	if (!(aht_status & AHT20_CALIBRATION_NEEDED))
		return (_aht_calibration());

	return (0);
}

static int8_t	_aht_calibration(void)
{
	uart_print_str("AHT20: calibration");
	// calibrate
	if (i2c_start(AHT20_ADDRESS, MODE_WRITE))
		return (1);
	if (i2c_write(AHT20_CALIBRATE)
		|| i2c_write(0x08)
		|| i2c_write(0x00))
	{
		i2c_stop();
		return (1);
	}
	i2c_stop();
	_delay_ms(AHT20_CALIBRATION_DELAY);
	return (0);
}


// .... Measure ....

static int8_t	aht20_command_measure(void);
static int8_t	aht20_read_measure_data(uint8_t	*measure);
static void		aht20_interpret_measure_data(uint8_t *measure);


int8_t	aht20_measure(void)
{
	uint8_t	measure[8];

	if (aht20_command_measure() || aht20_read_measure_data(measure))
		return (1);
	aht20_interpret_measure_data(measure);
	return (0);
}


static int8_t	aht20_command_measure(void)
{
	if (i2c_start(AHT20_ADDRESS, MODE_WRITE))
		return (1);
	if (i2c_write(AHT20_MEASURE)
		|| i2c_write(0x33)
		|| i2c_write(0x00))
	{
		i2c_stop();
		return (1);
	}
	i2c_stop();
	_delay_ms(AHT20_MEASURE_DELAY);
	_delay_ms(AHT20_MEASURE_SECURITY_DELAY);
	return (0);
}

static int8_t	aht20_read_measure_data(uint8_t	*measure)
{

	if (i2c_start(AHT20_ADDRESS, MODE_READ))
		return (1);
	if (i2c_read(ACK, &measure[0]))
	{
		i2c_stop();
		return (1);
	}
	if (!(TWDR & AHT20_MEASURE_ONGOING_MASK))
	{
		for (unsigned int i = 1; i < 6; ++i)
		{
			if (i2c_read(ACK, measure + i))
			{
				i2c_stop();
				return (1);
			}
		}
		if (i2c_read(NACK, measure + 6))
		{
			i2c_stop();
			return (1);
		}
		i2c_stop();
		return (0);
	}
	else
	{
		uart_print_str("Error: AHT20 sensor: measure took too long\r\n");
		return (1);
	}
}

static void	aht20_interpret_measure_data(uint8_t *measure)
{
	for (unsigned int i = 0; i < 7; ++i)
	{
		uart_print_hexa(measure[i]);
		uart_tx(' ');
	}
	uart_print_str("\r\n");
}