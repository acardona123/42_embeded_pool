# include "main.h"

#define DUTTY_CYCLE	0.1

void	cpt_init(void);

int main( void )
{
	SET(DDRB, D2);
	cpt_init();

	while (1)
	{}

	return (0);
}

void	cpt_init(void)
{	
	//predivider // divide by 256
	RESET(TCCR1B, CS10);
	RESET(TCCR1B, CS11);
	SET(TCCR1B, CS12); 

	//set comparisons
	RESET(TCCR1A, COM1A0); //Clear OC1A/OC1B on Compare Match when up-counting. Set OC1A/OC1B on Compare Match when downcounting.
	SET(TCCR1A, COM1A1);

	//wave generation mode bit description : Fast PWM 14
	RESET(TCCR1A, WGM10);
	SET(TCCR1A, WGM11);
	SET(TCCR1B, WGM12);
	SET(TCCR1B, WGM13);

	ICR1 = 62500;
	OCR1A = 62500 * DUTTY_CYCLE;
}

