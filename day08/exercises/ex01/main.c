#include "main.h"

int main()
{
	spi_led_color	d6_colors, d_off;

	spi_master_init();

	d6_colors = spi_led_generate_colors(01, 0xFF, 0, 0);
	d_off = spi_led_generate_colors(0, 0, 0, 0); 

	while (1)
	{
		spi_led_command(spi_led_generate_colors(01, 0xFF, 0, 0), d_off, d_off);
		_delay_ms(1000);

		spi_led_command(spi_led_generate_colors(01, 0, 0xFF, 0), d_off, d_off);
		_delay_ms(1000);
		
		spi_led_command(spi_led_generate_colors(01, 0, 0, 0xFF), d_off, d_off);
		_delay_ms(1000);
		
		spi_led_command(spi_led_generate_colors(01, 0xFF, 0xFF, 0), d_off, d_off);
		_delay_ms(1000);
		
		spi_led_command(spi_led_generate_colors(01, 0, 0xFF, 0xFF), d_off, d_off);
		_delay_ms(1000);
		
		spi_led_command(spi_led_generate_colors(01, 0xFF, 0, 0xFF), d_off, d_off);
		_delay_ms(1000);
		
		spi_led_command(spi_led_generate_colors(01, 0xFF, 0xFF, 0xFF), d_off, d_off);
		_delay_ms(1000);
	}
	return (0);
}