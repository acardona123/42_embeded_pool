// Analog to Digital Conversion Module

/*
sources:

https://ece-classes.usc.edu/ee459/library/documents/ADC.pdf
ATMega328P doc
*/

#include "ADC.h"

unsigned char adc_RV1_8()
{
	DDRC &= ~(1 << ADC_POT);
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADMUX |= ADC0 ;//ADC_pot = ADC0 = 0
	ADCSRA = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADCSRA |= (1 << ADEN);

	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADSC)) > 0);
	return (ADCH);
}