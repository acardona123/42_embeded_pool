# include "main.h"

#define DELAY 3000			

volatile unsigned long int cpt; //volatile to share  the variable through the code, across the main and the interrupt

int main( void )
{
	// set the pin directions for DELs and switches
	SET(DDRB, D1);
	RESET(DDRD, SW1);
	RESET(DDRD, SW2);

	/* register description: interrupt setup (doc ATmega... 12.5.1) */

		//Moving Interrupts Between Application and Boot Space
	//? IVSEL and IVCE    in MCUCR

	//Enableling the global interrupt enable (doc 7.3.1)
		SET(SREG, 7); 

	//external trigger with  SW1 (pin INT0)
		//configure what will trigger the interrupt (here rising edge). (Table 13-1)
		SET(EICRA, ISC00); 
		SET(EICRA, ISC01);
		// configure the mask to set what pin triggers the interrupt -> INT0
		SET(EIMSK, INT0);
		RESET(EIMSK, INT1);
		// configure the pin change mask (13.2.8)
		SET(PCMSK0, PCINT0); //see 14.3.1 -> PCINT0 activate PB0
		// define the action to do in case of interruption
	
	//run the programme
	while (1)
	{
		if (cpt < DELAY) //delay to avoid the bounce effect
			++cpt;
	}
	return (0);
}

ISR(INT0_vect)
{
	if ( cpt == DELAY )
	{
		TOGGLE(PORTB, D1);
		cpt = 0;
	}
}

