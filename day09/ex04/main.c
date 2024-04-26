#include "main.h"


//16-Bit I/O Expander

int main()
{
	uart_init();
	i2c_init();
	io_exp_init();

	while (1)
	{
		io_display_digit(1, 4);
		_delay_ms(1);
		io_exp_write_in_register(IO_OUT_1, 0);
		io_display_digit(2, 2);
		_delay_ms(1);
		io_exp_write_in_register(IO_OUT_1, 0);
	}
	return (0);
}