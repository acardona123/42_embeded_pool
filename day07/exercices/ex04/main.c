#include "main.h"

static void	_display_led_ratio(unsigned char c);

int main ( void )
{
	unsigned char c;

	rgb_init();
	DDRB |= (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4);

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
	PORTB = PORTB & (~((1 << D1) | (1 << D2) | (1 << D3) | (1 << D4)))
			| ((c >=64) << D1)
			| ((c >=128) << D2)
			| ((c >=192) << D3)
			| ((c == 255) << D4);
}
