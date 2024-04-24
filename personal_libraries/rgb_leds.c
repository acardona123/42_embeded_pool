#include "rgb_leds.h"

static void	_timer2_init() //OC2B->BLUE
{	
	sei();
	//predivider // divide by 1024 ==> 15625
	SET(TCCR2B, CS20);
	SET(TCCR2B, CS21);
	SET(TCCR2B, CS22); 

	//set comparisons; //OC2B triggers te DEL, OC2A do nothing ==> Fast PWMMode
		//OC2A ignored
		RESET(TCCR2A, COM2A0);  //Table 18-6. Compare Output Mode, Fast PWM Mode
		RESET(TCCR2A, COM2A1);
		//OC2B BLUE, inverse mode to be able to get 0
		SET(TCCR2A, COM2B0); // in 3 of Table 18-6. Compare Output Mode, Fast PWM Mode
		SET(TCCR2A, COM2B1);

	//wave generation mode bit description :  Fast PWMMode , line 3 ofTable 18-8. Waveform Generation Mode Bit Description
	SET(TCCR2A, WGM20);
	SET(TCCR2A, WGM21);
	RESET(TCCR2B, WGM22);

	OCR2B = 0;
}

static void	_timer0_init() //OCR0A-> green, OCR0B ->red
{	
	sei();
	//predivider // divide by 1024 ==> 15625
	SET(TCCR0B, CS00);
	// RESET(TCCR0B, CS01);
	// SET(TCCR0B, CS02); 

	//set comparisons; 
		//OC2A GREEN, inverse mode to be able to get 0
	SET(TCCR0A, COM0A0);  //Table 15-3. Compare Output Mode, Fast PWM Mode
	SET(TCCR0A, COM0A1);
		//OC2B RED, inverse mode to be able to get 0
	SET(TCCR0A, COM0B0); //Table 18-6. Compare Output Mode, Fast PWM Mode, Clear OC0B on Compare Match, set OC0B at BOTTOM
	SET(TCCR0A, COM0B1);

	//wave generation mode bit description :  Fast PWMMode , line 3 ofTable 18-8. Waveform Generation Mode Bit Description
	SET(TCCR0A, WGM00);
	SET(TCCR0A, WGM01);
	RESET(TCCR0B, WGM02);

	//max is 0xFF
	OCR0A = 0;
	OCR0B = 0;
}

void rgb_init()
{
	DDRD |= (1 << RGB_B) | (1 << RGB_G) | (1 << RGB_R);
	PORTD &= ~((1 << RGB_B) | (1 << RGB_G) | (1 << RGB_R));
	_timer0_init();
	_timer2_init();
	rgb_set(0, 0, 0);
}

void rgb_set(uint8_t red, uint8_t green, uint8_t blue)
{
	OCR0A = 255 - green;
	OCR0B = 255 - red;
	OCR2B = 255 - blue;
}


void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85)
	{
		rgb_set(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170) {
		pos = pos - 85;
		rgb_set(0, pos * 3, 255 - pos * 3);
	}
	else {
		pos = pos - 170;
		rgb_set(pos * 3, 255 - pos * 3, 0);
	}
}