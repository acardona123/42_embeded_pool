#include "main.h"

void	timer1_init(void);

volatile uint16_t	cpt;

//16-Bit I/O Expander

int main()
{
	sei();
	uart_init();
	i2c_init();
	io_exp_init();
	timer1_init();

	cpt = 0;
	while (1)
	{
		io_display_number(cpt);
	}
	return (0);
}

void	timer1_init(void) //commands the modification of the del cycle dutty at 100 Hz
{	
	//predivider // divide by 1024 (obtained by using the formula at the end of 15.7.2 and trying to have a OCRA that would not overflow), table 15.9
	SET(TCCR1B, CS10);
	RESET(TCCR1B, CS11);
	SET(TCCR1B, CS12); 

	//wave generation mode bit description (table 15-8): CTC
	RESET(TCCR1A, WGM10);
	RESET(TCCR1A, WGM11);
	SET(TCCR1B, WGM12);
	RESET(TCCR1B, WGM13);

	//set comparisons (Table 15-2) CTC mode, -> disconnect
	RESET(TCCR1A, COM1A0);
	RESET(TCCR1A, COM1A1);
	OCR1A = 15624;//(obtained by using the formula at the end of 15.7.2 and trying to have a OCRA that would not overflow) /!\ We only detect one state change, so we do not divide by 2 in the formula
				// = f_clk/(N * f_OCnA) - 1

	//interrupt flags : 15.9.6
	SET(TIMSK1, ICIE1);
	SET(TIMSK1, OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	if (++cpt == 10000)
		cpt = 0;
}