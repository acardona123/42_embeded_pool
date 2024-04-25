#include "main.h"

int main()
{
	spi_led_color	d_colors, d_off;

	spi_master_init();

	d_colors = spi_led_generate_colors(01, 0xFF, 0, 0);
	d_off = spi_led_generate_colors(0, 0, 0, 0); 

	while (1)
	{
		spi_led_command(spi_led_generate_colors(1, 0xFF, 0, 0), d_off, d_off);
		_delay_ms(225);

		spi_led_command(d_off, spi_led_generate_colors(1, 0xFF, 0, 0), d_off);
		_delay_ms(225);
		
		spi_led_command(d_off, d_off, spi_led_generate_colors(1, 0xFF, 0, 0));
		_delay_ms(225);
		
		spi_led_command(d_off, d_off, d_off);
		_delay_ms(225);
	}
	return (0);
}