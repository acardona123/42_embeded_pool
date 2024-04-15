# include "main.h"

int main( void )
{
	//objectiv: set the port A0 to 1, all the others to 0;
	DDRB = 1 << PB0; //set the pin A0 as an output pin (others are input by default)
	PORTB = 1 << PB0; // set the output value of A0 to 1
	return (0);
}