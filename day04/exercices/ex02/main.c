#include "main.h"

int main()
{
	init_USART();

	uart_print_str("=====\r\nSTART\r\n====\r\n\n");

	i2c_init();

	if (aht20_init())
	{
		uart_print_str("AHT20 initialisation fatal error\r\n");
		_delay_ms(1000);
		return (1);
	}

	float	temperature;
	float	humidity;

	while (1)
	{
		if (aht20_measure(&temperature, &humidity))
		{
			uart_print_str("AHT20 measure fatal error\r\n");
			_delay_ms(1000);
			return (1);
		}
		print_temperature_humidity(temperature, humidity);
		_delay_ms(1000);
	}
	return (0);
}

void	print_temperature_humidity(float temperature, float humidity)
{
	char str[10] = {0};

	uart_print_str("Temperature: ");
	dtostrf(temperature, 0, 1, str);
	uart_print_str(str);
	uart_print_str("°C, Humidity: ");
	dtostrf(humidity, 0, 0, str);
	uart_print_str(str);
	uart_print_str("%\n\r");
}