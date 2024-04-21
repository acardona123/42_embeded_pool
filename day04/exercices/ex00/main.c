# include "main.h"

//sensor : https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf

int main()
{
	init_USART();
	_i2c_init();
	_delay_ms(THERMO_SENSOR_INIT_TIME * 2);
	i2c_start();
	i2c_stop();

	return (0);
}

void	i2c_start( void )//start+_i2c_send_SLA
{
	_i2c_start(THERMO_SENSOR_ADDRESS, MODE_WRITE);
}

void	i2c_stop( void )//start+_i2c_send_SLA
{
	_i2c_stop();
}
