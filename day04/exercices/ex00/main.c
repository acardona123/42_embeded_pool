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
	_i2c_start();
	uart_printstr("Start: ");
	i2c_print_status();
	_i2c_send_SLA(THERMO_SENSOR_ADDRESS, MODE_WRITE);
	uart_printstr("Send SLA: ");
	i2c_print_status();
}

void	i2c_stop( void )
{
	_i2c_stop();
	uart_printstr("Stop: ");
	i2c_print_status();
}

void i2c_print_status( void )
{
	uart_printstr("0x");
	uart_printhexa(TW_STATUS);
	uart_printstr("\r\n");
}
