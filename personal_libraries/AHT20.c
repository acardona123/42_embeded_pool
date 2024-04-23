#include "AHT20.h"


/* 
====
 Init
====
*/


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


/* 
====
 Take measure
====
*/

// .... Measure ....

static int8_t	aht20_command_measure(void);
static int8_t	aht20_read_measure_data(uint8_t	*measure);
static void		aht20_get_temperature_humidity_from_raw_data(
					uint8_t *measure_raw_data, float *temperature, float *humidity);


int8_t	aht20_measure(float *temperature, float *humidity)
{
	uint8_t	measure_raw_data[8];

	if (aht20_command_measure() || aht20_read_measure_data(measure_raw_data))
		return (1);
	aht20_get_temperature_humidity_from_raw_data(measure_raw_data, temperature, humidity);
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

static int8_t	aht20_read_measure_data(uint8_t	*measure_data)
{


	if (i2c_start(AHT20_ADDRESS, MODE_READ))
		return (1);
	if (i2c_read(ACK, &measure_data[0]))
	{
		i2c_stop();
		return (1);
	}
	if (!(TWDR & AHT20_MEASURE_ONGOING_MASK))
	{
		for (unsigned int i = 1; i < 6; ++i)
		{
			if (i2c_read(ACK, measure_data + i))
			{
				i2c_stop();
				return (1);
			}
		}
		if (i2c_read(NACK, measure_data + 6))
		{
			i2c_stop();
			return (1);
		}
		i2c_stop();
		return (0);
	}
	else
	{
		i2c_stop();
		uart_print_str("Error: AHT20 sensor: measure took too long\r\n");
		return (1);
	}
}

static void	aht20_get_temperature_humidity_from_raw_data(uint8_t *measure_raw_data, float *temperature, float *humidity)
{
	uint32_t temperature_signal;
	uint32_t humidity_signal;

	//first 20 bits of read_buff + 1 
	humidity_signal = measure_raw_data[1];
	humidity_signal	<<= 8;
	humidity_signal += measure_raw_data[2];
	humidity_signal	<<= 4;
	humidity_signal += (measure_raw_data[3] >> 4);
	*humidity = (float)(humidity_signal * 100)/TWO_POWER_TWENTY;

	//first 20 nits after the second half of measure_raw_data+3
	temperature_signal = (measure_raw_data[3]) & 0x0F;
	temperature_signal <<= 8;
	temperature_signal += measure_raw_data[4];
	temperature_signal <<= 8;
	temperature_signal += measure_raw_data[5];
	*temperature = (float)(temperature_signal *  200) / TWO_POWER_TWENTY - 50.f;
}



/* 
====
 take Averaged measure
====
*/


static void	_init_temperature_humidity_historic(
			float *temperature_historic, float *humidity_historic,
			float new_temperature, float new_humidity);
static void	_update_temperature_humidity_historic(
				float *temperature_historic, float *humidity_historic,
				float new_temperature, float new_humidity);
static float	_calculate_average(float *f_trio);

int8_t	aht20_averaged_measure(float *temperature, float *humidity)
{
	static bool		init = true;
	static float	temperature_historic[3] = {};
	static float	humidity_historic[3] = {};
	float 			new_temperature;
	float 			new_humidity;

	if (aht20_measure(&new_temperature, &new_humidity))
		return (1);
	if (init)
	{
		_init_temperature_humidity_historic(
			temperature_historic, humidity_historic,
			new_temperature, new_humidity);
		init = false;
	}
	else
	{
		_update_temperature_humidity_historic(
			temperature_historic, humidity_historic,
			new_temperature, new_humidity);
	}
	*temperature = _calculate_average(temperature_historic);
	*humidity = _calculate_average(humidity_historic);
	return (0);
}

//... 

static void	_init_trio_field(float *historic, float new_value);

static void	_init_temperature_humidity_historic(float *temperature_historic, float *humidity_historic, float new_temperature, float new_humidity)
{
	_init_trio_field(temperature_historic, new_temperature);
	_init_trio_field(humidity_historic, new_humidity);
}

static void	_init_trio_field(float *historic, float new_value)
{
	historic[1] = historic[2] = historic [0] = new_value;
}


//...

static void	_update_trio_field(float *historic, float new_value);

static void	_update_temperature_humidity_historic(
				float *temperature_historic, float *humidity_historic,
				float new_temperature, float new_humidity)
{
	_update_trio_field(temperature_historic, new_temperature);
	_update_trio_field(humidity_historic, new_humidity);
}

static void	_update_trio_field(float *historic, float new_value)
{
	historic[0] = historic[1];
	historic[1] = historic[2];
	historic[2] = new_value;
}


//...

static float	_calculate_average(float *f_trio)
{
	return ((f_trio[0] + f_trio[1] + f_trio[2]) / 3);
}

//...

