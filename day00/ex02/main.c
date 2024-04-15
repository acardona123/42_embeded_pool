# include "main.h"

int main( void )
{
	DDRD = 0; 								// set the pin D2 (and all the other ones) as an output pin (already is by default)
	DDRB = 1 << PB0;						//set the pin A0 as an output pin (others are input by default)
	PORTB = 0;								// set the output value of A0 to 1
	while ( 1 )
	{
		while (PIND & (1 << PD2))			 //tests if the switch is pressed
			_delay_ms(1);			 		//wait before each test
		PORTB = 1 << PB0;			 		// set the output value of A0 to 1 to turn on the led
		while (! (PIND & (1 << PD2)))		//tests if the switch s released
			_delay_ms(1);
		PORTB = 0;			 				// set the output value of A0 to 1 to turn off the led
	}
	return (0);
}