#include "main.h"

static void	_display_led_ratio(unsigned char c);

int main ( void )
{
	unsigned char c;

	rgb_init();

	while(1)
	{
		c = adc_RV1_8();
		wheel(c);
		_display_led_ratio(c);
		_delay_ms(20);
	}
	
	return (0);
}

static void	_display_led_ratio(unsigned char c)
{
	if (c >= 255 / 4)
		SET(PORTB, D1);
	else
		RESET(PORTB, D1);
	if (c >= 255 / 2)
		SET(PORTB, D2);
	else
		RESET(PORTB, D2);
	if ((uint16_t)c * 4 / 3 >= 255)
		SET(PORTB, D3);
	else
		RESET(PORTB, D3);
	if (c == 255)
		SET(PORTB, D4);
	else
		RESET(PORTB, D4);
}
