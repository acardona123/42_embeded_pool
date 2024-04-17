# include "main.h"

# define FREQUENCY_CPT0	200 //Hz
# define FREQUENCY_CPT1	100 //Hz
# define DUTTY_CYCLE_MIN 0
# define DUTTY_CYCLE_MAX 100
# define DUTTY_CYCLE_STEP 1

volatile int	g_dutty_cycle_percent = 1;
volatile bool	g_dutty_cycle_direction = INCREASING;

void	timer1_init(void);
void	timer0_init(void);
void	interrupt_init( void );

int main( void )
{
	SET(DDRB, D2);

	//tests
	// SET(DDRB, D1);//
	// SET(DDRB, D3);//
	// SET(PORTB, D3);//

	timer1_init();
	timer0_init();
	interrupt_init();
	while (1)
	{}
	return (0);
}


/*
Initialisation of the Timer1
*/

static void	update_OCR1A( void )
{
	OCR1A = ICR1 * g_dutty_cycle_percent / 100;
}

void	timer1_init(void) //commands the led wit a cycle dutty
{	
	//predivider // divide by 256 ==> 62500
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

	ICR1 = (int)(62500.f / (float)FREQUENCY_CPT1);
	update_OCR1A();
}


/*
Initialisation of the Timer0
*/

static void	update_OCR0A( void )
{
	OCR1A = ICR1 * g_dutty_cycle_percent / 100;
}

void	timer0_init(void) //commands the modification of the del cycle dutty at 200 Hz
{	
	//predivider // divide by 1024 (obtained by using the formula at the end of 15.7.2 and trying to have a OCRA that would not overflow), table 15.9
	SET(TCCR0B, CS00);
	RESET(TCCR0B, CS01);
	SET(TCCR0B, CS02); 

	//wave generation mode bit description (table 15-8): CTC
	RESET(TCCR0A, WGM00);
	SET(TCCR0A, WGM01);
	RESET(TCCR0B, WGM02);

	//set comparisons (Table 15-2) CTC mode, -> disconnect
	RESET(TCCR0A, COM0A0);
	RESET(TCCR0A, COM0A1);
	OCR0A = 77;//(obtained by using the formula at the end of 15.7.2 and trying to have a OCRA that would not overflow) /!\ We only detect one state change, so we do not divide by 2 in the formula

	//interrupt flags : 15.9.6
	SET(TIMSK0, OCIE0A);
	SET(TIFR0, OCF0A);
}


/*
Initialisation of the interrupt on Timer0 compare equality
*/

void	interrupt_init( void )
{
	//everything is included in the timer
	//Enableling the global interrupt enable (doc 7.3.1)
	SET(SREG, 7); 
}

static void	_increment_dutty_cycle( void );
static void	_decrement_dutty_cycle( void );
void		modify_dutty_cycle( void );

ISR(TIMER0_COMPA_vect)
{
	modify_dutty_cycle();
	update_OCR1A();
}

void	modify_dutty_cycle( void )
{
	if (g_dutty_cycle_direction == DECREASING)
		_decrement_dutty_cycle();
	else
		_increment_dutty_cycle();
}

static void	_increment_dutty_cycle( void )
{
	g_dutty_cycle_percent += DUTTY_CYCLE_STEP;
	if (g_dutty_cycle_percent >= DUTTY_CYCLE_MAX)
	{
		// TOGGLE(PORTB, D3);//
		// TOGGLE(PORTB, D1);//
		g_dutty_cycle_direction = DECREASING;
		g_dutty_cycle_percent = DUTTY_CYCLE_MAX;
	}
}
static void	_decrement_dutty_cycle( void )
{
	g_dutty_cycle_percent -= DUTTY_CYCLE_STEP;
	if (g_dutty_cycle_percent <= DUTTY_CYCLE_MIN)
	{
		// TOGGLE(PORTB, D3);//
		// TOGGLE(PORTB, D1);//
		g_dutty_cycle_direction = INCREASING;
		g_dutty_cycle_percent = DUTTY_CYCLE_MIN;
	}
}