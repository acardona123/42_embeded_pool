#include "main.h"

static void	_disp_binary_color(bool R, bool G, bool B);

int main()
{
	DDRD = (1 << RGB_B) | (1 << RGB_G) | (1 << RGB_R);
	PORTD = 0;

	while (1)
	{
		_disp_binary_color(1, 0, 0);
		_delay_ms(1000);
		_disp_binary_color(0, 1, 0);
		_delay_ms(1000);
		_disp_binary_color(0, 0, 1);
		_delay_ms(1000);
		_disp_binary_color(1, 1, 0);
		_delay_ms(1000);
		_disp_binary_color(0, 1, 1);
		_delay_ms(1000);
		_disp_binary_color(1, 0, 1);
		_delay_ms(1000);
		_disp_binary_color(1, 1, 1);
		_delay_ms(1000);
	}
}

static void	_disp_binary_color(bool R, bool G, bool B)
{
	PORTD &= ~ ((1 << RGB_B) | (1 << RGB_G) | (1 << RGB_R));
	PORTD |= ((1 << RGB_R) * R) | ((1 << RGB_G) * G) | ((1 << RGB_B) * B);
}