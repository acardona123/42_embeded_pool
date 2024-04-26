#include "main.h"


//16-Bit I/O Expander

int main()
{

	uart_init();
	i2c_init();
	io_exp_init();

	io_display_digit(4, 2);
	while (1);
	return (0);
}