#include "main.h"

int main()
{
	spi_led_color	d6_colors, d7_colors, d8_colors;


	uart_init();
	spi_master_init();

	d6_colors = spi_led_generate_colors(01, 0xFF, 0, 0);
	d7_colors = spi_led_generate_colors(0, 0, 0, 0); 
	d8_colors = spi_led_generate_colors(0, 0, 0, 0);

	spi_led_command(d6_colors, d7_colors, d8_colors);
	while (1);
	return (0);
}