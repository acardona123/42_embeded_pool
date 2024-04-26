#include "main.h"


//16-Bit I/O Expander

int main()
{
	uint8_t	cpt;

	uart_init();
	i2c_init();
	io_exp_init();

	cpt = 0;
	while (1)
	{
		io_display_digit(4, cpt);
		cpt = (cpt + 1) % 10;
		_delay_ms(1000);
	}
	return (0);
}