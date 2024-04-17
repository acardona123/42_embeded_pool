# include "main.h"

#define REAL_FCPU	1203000 //real fcpu = 0-20MHz

int	g_dutty_cycle_percent = 10;

void	cpt_init(void);
void	increment_dutty_cycle( void );
void	decrement_dutty_cycle( void );



int main( void )
{

	sw_status	sw1_status, sw2_status;

	DDRD = 0;								//setting the D register to "listen" all the ports
	sw1_status = get_sw_status(PIND, SW1);	//get the current status of switch 1
	sw2_status = get_sw_status(PIND, SW2);	//get the current status of switch 2
	
	SET(DDRB, D2); // sets the  pin of D2as outputs
	
	
	cpt_init();

	while (1)
	{
		if (get_sw_status(PIND, SW1) != sw1_status)
		{
			_delay_ms(50);
			sw1_status ^= 1;
			if (sw1_status == SW_ON)
			{
				increment_dutty_cycle();
			}
		}
		if (get_sw_status(PIND, SW2) != sw2_status)
		{
			_delay_ms(50);
			sw2_status ^= 1;
			if (sw2_status == SW_ON)
			{
				decrement_dutty_cycle();
			}
		}
	}
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
	OCR1A = (62500 / 100) * g_dutty_cycle_percent;
}

void	increment_dutty_cycle( void )
{
	if (g_dutty_cycle_percent < 100)
		g_dutty_cycle_percent += 10;
	OCR1A = (62500 / 100) * g_dutty_cycle_percent;
}
void	decrement_dutty_cycle( void )
{
	if (g_dutty_cycle_percent > 10)
		g_dutty_cycle_percent -= 10;
	OCR1A = (62500 / 100) * g_dutty_cycle_percent;

}
