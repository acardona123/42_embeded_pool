# include "main.h"

#define NUMBER_OF_CYCLES_PER_LOOP	4
#define TARGET_FREQUENCY			0.5f
#define CORRECTOR					25

static const unsigned long long int target_cpt = F_CPU / NUMBER_OF_CYCLES_PER_LOOP * TARGET_FREQUENCY / CORRECTOR;

int main( void )
{
	DDRB = 1 << D2;
	PORTB = 0;

	unsigned long long int cpt;
	cpt = 0;
	while (1)
	{
		if (cpt == target_cpt)
		{
			PORTB ^= (1 << PB1);
			cpt = 0;
		}
		++cpt;
	}
	return (0);
}

