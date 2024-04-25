#include "main.h"

static void	_display_led_ratio(unsigned char c);

int main ( void )
{
	unsigned char	c;
	spi_led_color	d_colors, d_off;

	spi_master_init();

	d_colors = spi_led_generate_colors(01, 0xFF, 0, 0);
	d_off = spi_led_generate_colors(0, 0, 0, 0); 

	while(1)
	{
		c = adc_RV1_8();
		spi_led_command((c >= 85) ? d_colors : d_off, (c >= 170) ? d_colors : d_off, (c == 255) ? d_colors : d_off);
		_delay_ms(20);
	}
	
	return (0);
}
