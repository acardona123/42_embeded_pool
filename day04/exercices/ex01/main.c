#include "main.h"

int main()
{
	init_USART();

	uart_print_str("=====\r\nSTART\r\n====\r\n\n");

	_i2c_init();

	if (aht20_init())
	{
		uart_print_str("AHT20 initialisation fatal error\r\n");
		_delay_ms(1000);
		return (1);
	}

	while (1)
	{
		if (aht20_measure())
		{
			uart_print_str("AHT20 measure fatal error\r\n");
			_delay_ms(1000);
			return (1);
		}
		_delay_ms(1000);
	}
	return (0);
}
