# include "main.h"


int main( void )
{

	
	DDRB = 1 << D2;
	PORTB = 0;

	unsigned long long int cpt;
	cpt = 0;
	while (1) //
	{
		if (cpt == 190000) //
		{
			PORTB ^= (1 << PB1);
			cpt = 0;
		}
		++cpt; // 1 cycle
	}
	return (0);
}

