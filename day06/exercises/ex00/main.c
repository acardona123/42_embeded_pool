#include "main.h"

int main()
{
	DDRD = (1 << RGB_B) | (1 << RGB_G) | (1 << RGB_R);
	PORTD = 0;

	while (1)
	{
		SET(PORTD, RGB_R);
		_delay_ms(1000);
		RESET(PORTD, RGB_R);
		SET(PORTD, RGB_G);
		_delay_ms(1000);
		RESET(PORTD, RGB_G);
		SET(PORTD, RGB_B);
		_delay_ms(1000);
		RESET(PORTD, RGB_B);
	}
}