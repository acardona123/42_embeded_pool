#include "main.h"


//16-Bit I/O Expander

int main()
{
	char i = 0;

	uart_init();
	i2c_init();
	io_exp_init();

	while (1)
	{
		io_exp_cmd_D9(++i % 2);
		_delay_ms(500);
	}

	return (0);
}
