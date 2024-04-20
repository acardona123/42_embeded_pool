# include "main.h"

//sensor : https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf

int main()
{
	init_USART();
	_i2c_init();
	sensor_temp_read_in_loop();
	return (0);
}


// ==== temperatur sensor ====

e_success	sensor_temp_init( void );
e_success	sensor_temp_take_measure(float *temperature, float *humidity );

void		init_temperature_humidity_historic(
				float *temperature_historic, float *humidity_historic,
				float new_temperature, float new_humidity);
void		update_temperature_humidity_historic(
				float *temperature_historic, float *humidity_historic,
				float new_temperature, float new_humidity);
void		print_humidity_temperature(float *temperature_historic, float *humidity_historic);


void	sensor_temp_read_in_loop( void )
{
	float temperature_historic[3];
	float humidity_historic[3];
	float new_temperature;
	float new_humidity;


	if (sensor_temp_init() == FAILURE
		|| sensor_temp_take_measure(&new_temperature, &new_humidity) == FAILURE)
		return;
	init_temperature_humidity_historic(temperature_historic, humidity_historic,
		new_temperature, new_humidity);
	print_humidity_temperature(temperature_historic, humidity_historic);

	while (1)
	{
		if (sensor_temp_take_measure(&new_temperature, &new_humidity) == FAILURE)
		{
			continue;
		}
		update_temperature_humidity_historic(temperature_historic, humidity_historic,
			new_temperature, new_humidity);
		print_humidity_temperature(temperature_historic, humidity_historic);
	}
}


//...................


e_success	check_and_do_calibration( void );

e_success sensor_temp_init( void )
{
	char	read_buff[THERMO_SENSOR_NUMBER_OF_BYTE_PER_MEASURE + 1] = {};
	int		idx_in_buff = 0;

	_i2c_start();
	_delay_ms(THERMO_SENSOR_INIT_TIME * 2);
	return (check_and_do_calibration());
}


//...................


e_success	check_and_do_calibration( void ) // doc 5.4 step 1
{
	char	received_byte;

	if (_i2c_send_SLA(THERMO_SENSOR_ADDRESS, MODE_READ) == FAILURE)
	{
		_i2c_stop();
		return (FAILURE);
	}
	received_byte = _i2c_receiving_data(false);
	if (received_byte & (1 << THERMO_SENSOR_BIT_CALIBRATION_ENABLED))
	{
		if (_i2c_repeated_start() == FAILURE
			|| _i2c_send_SLA(THERMO_SENSOR_ADDRESS, MODE_WRITE) == FAILURE
			|| _i2c_sending_data_char(0xBE) == FAILURE
			|| _i2c_sending_data_char(0x08)
			|| _i2c_sending_data_char(0))
		{
			_i2c_stop();
			return (FAILURE);
		}
		_delay_ms(10);
	}
	return (SUCCESS);
}


//...................


e_success	_cmd_start_measure( void );
e_success	_wait_during_measure( void );
void		_read_measure( char * read_buff );
void		_calculate_temperature_humidity(char *read_buff, float *temperature, float *humidity);

e_success	sensor_temp_take_measure(float *temperature, float *humidity )
{
	char	 read_buff[THERMO_SENSOR_NUMBER_OF_BYTE_PER_MEASURE + THERMO_SENSOR_CHECK_CRC] = {};
	
	//connexion must have been started
	if (_cmd_start_measure() == FAILURE
		|| _wait_during_measure() == FAILURE)
		return (FAILURE);
	_read_measure(read_buff);
	_i2c_stop();
	// if (THERMO_SENSOR_CHECK_CRC)
	// {
	// 	//check_CRC
	// }
	_calculate_temperature_humidity(read_buff, temperature, humidity);
	return (SUCCESS);
}

e_success	_cmd_start_measure( void ) // doc 5.4 step 2
{
	//
	if (_i2c_get_RW_status() == MODE_READ //in this case we want to switch to write mode
				&& (_i2c_repeated_start() == FAILURE
					|| _i2c_send_SLA(THERMO_SENSOR_ADDRESS, MODE_WRITE) == FAILURE)
			//then in any case :
			|| _i2c_sending_data_char(0xAC) == FAILURE
			|| _i2c_sending_data_char(0x33)
			|| _i2c_sending_data_char(0)
			)
	{
		_i2c_stop();
		return (FAILURE);
	}
	return (SUCCESS);
}

e_success	_wait_during_measure( void ) // doc 5.4 step 3
{
	char	read_byte;

	_delay_ms(80);
	if (_i2c_repeated_start() == FAILURE
		|| _i2c_send_SLA(THERMO_SENSOR_ADDRESS, MODE_READ) == FAILURE)
	{
		_i2c_stop();
		return (FAILURE);
	}
	while (TWDR & (1 << 7))
	{
		_delay_ms(10);
	}
	return (SUCCESS);

	// char	read_byte;

	// _delay_ms(80);
	// do 
	// {
	// 	if (_i2c_repeated_start() == FAILURE
	// 		|| _i2c_send_SLA(THERMO_SENSOR_ADDRESS, MODE_READ) == FAILURE)
	// 	{
	// 		_i2c_stop();
	// 		return (FAILURE);
	// 	}
	// 	read_byte = _i2c_receiving_data(true);
	// 	if (read_byte & (1 << 7))
	// 	{
	// 		_delay_ms(10);
	// 	}
	// } while (read_byte & (1 << 7));
	// return (SUCCESS);
}

void	_read_measure( char * read_buff) // doc 5.4 step 4
{
	int	idx_in_buff;

	idx_in_buff = 0;
	while (idx_in_buff < THERMO_SENSOR_NUMBER_OF_BYTE_PER_MEASURE + THERMO_SENSOR_CHECK_CRC - 1)
	{
		read_buff[idx_in_buff] = _i2c_receiving_data(false);
		++idx_in_buff;
	}
	read_buff[idx_in_buff] = _i2c_receiving_data(true);
}

void	_calculate_temperature_humidity(char *read_buff, float *temperature, float *humidity)
{
	long long int temperature_signal;
	long long int humidity_signal;

	//first 20 bits of read_buff + 1 
	humidity_signal = (((long long int)read_buff[1]) << 12) + (((long long int)read_buff[2]) << 4) + (((long long int)read_buff[3]) >> 4);
	//first 20 nits after the second half of read_buff+3
	temperature_signal = (((long long int)(read_buff[3]) & 0x0F) << 16) + (((long long int)read_buff[4]) << 8) + read_buff[5];

	*humidity = (float)(humidity_signal * 100)/TWO_POWER_TWENTY;
	*temperature = (float)(temperature_signal *  200) / TWO_POWER_TWENTY - 50.f;
}


//...................


e_success	sensor_temp_soft_reset( void )
{
	e_success	success;

	if (_i2c_start() == FAILURE
		|| _i2c_send_SLA(THERMO_SENSOR_ADDRESS, MODE_WRITE) == FAILURE
		|| _i2c_sending_data_char(THERMO_SENSOR_CMD_sensor_temp_soft_reset) == FAILURE)
	{
		success = FAILURE;
	}
	else
	{
		success = SUCCESS;
	}
	_i2c_stop();
	return (SUCCESS);
}



// ==== Historic and display ====

void	_init_trio_field(float *historic, float new_value);

void	init_temperature_humidity_historic(float *temperature_historic, float *humidity_historic, float new_temperature, float new_humidity)
{
	_init_trio_field(temperature_historic, new_temperature);
	_init_trio_field(humidity_historic, new_humidity);
}

void	_init_trio_field(float *historic, float new_value)
{
	historic[1] = historic[2] = historic [0] = new_value;
}


//...................


void	_update_trio_field(float *historic, float new_value);

void	update_temperature_humidity_historic(
				float *temperature_historic, float *humidity_historic,
				float new_temperature, float new_humidity)
{
	_update_trio_field(temperature_historic, new_temperature);
	_update_trio_field(humidity_historic, new_humidity);
}

void	_update_trio_field(float *historic, float new_value)
{
	historic[0] = historic[1];
	historic[1] = historic[2];
	historic[2] = new_value;
}


//...................


float	_calculate_average(float *f_trio);
void		print_humidity_temperature(float *temperature_historic, float *humidity_historic)
{

	uart_print_str("Temperature: ");
	uart_print_float(_calculate_average(temperature_historic), 2);
	uart_print_str("°C, Humidity: ");
	uart_print_float(_calculate_average(humidity_historic), 3);
	uart_print_str("%\n\r");
}

float	_calculate_average(float *f_trio)
{
	return ((f_trio[0] + f_trio[1] + f_trio[2]) / 3);
}