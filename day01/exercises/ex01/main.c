# include "main.h"

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
	//setup the prescaler deactivated ()
	// CLKPCE = 0b10000000;
	// CLKPCE = 0; //devide the general clock frequency by 1

	//predivider // divide by 256
	RESET(TCCR1B, CS10);
	RESET(TCCR1B, CS11);
	SET(TCCR1B, CS12); 

	//set comparisons
	SET(TCCR1A, COM1A0);
	RESET(TCCR1A, COM1A1);
	OCR1A = 31249;

	//wave generation mode bit description
	RESET(TCCR1A, WGM10);
	RESET(TCCR1A, WGM11);
	SET(TCCR1B, WGM12);
	RESET(TCCR1B, WGM13);
}

