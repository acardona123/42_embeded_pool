# include "main.h"

int main( void )
{
	int	is_on = 0;

	DDRD = 0; 							// set the pin D2 (and all the other ones) as an output pin (already is by default)
	DDRB = 1 << PB0;					//set the pin A0 as an output pin (others are input by default)
	PORTB = 0;							// set the output value of A0 to 1
	while ( 1 )
	{
		while (PIND & (1 << PD2))		//tests if the switch is pressed
			_delay_ms(1);				//wait before each test
		_delay_ms(50);					//wait to avoid bounce (https://www.allaboutcircuits.com/technical-articles/switch-bounce-how-to-deal-with-it/)
		is_on ^= 1;
		PORTB = is_on << PB0;			// invert on/off
		while (! (PIND & (1 << PD2)))	//tests if the switch s released
			_delay_ms(1);
	}
	return (0);
}