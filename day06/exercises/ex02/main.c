#include "main.h"


int main( void )
{
	uint8_t	pos = 0;

	init_rgb();
	while (1)
	{
		wheel(pos);
		++pos;
		_delay_ms(10);
	}
	return (0);
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
	set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
	pos = pos - 85;
	set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
	pos = pos - 170;
	set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}