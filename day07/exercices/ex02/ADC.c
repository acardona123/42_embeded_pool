// Analog to Digital Conversion Module

/*
sources:

https://ece-classes.usc.edu/ee459/library/documents/ADC.pdf
ATMega328P doc
*/

#include "ADC.h"

//potentiometer
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

//luxmeter
unsigned char adc_LDR_8()
{
	DDRC &= ~(1 << ADC_LDR);
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADMUX |= ADC1 ;//ADC_pot = ADC0 = 0
	ADCSRA = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADCSRA |= (1 << ADEN);

	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADSC)) > 0);
	return (ADCH);
}

//adc thermo-resistor
unsigned char adc_NTC_8()
{
	DDRC &= ~(1 << ADC_NTC);
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADMUX |= ADC2 ;//ADC_pot = ADC0 = 0
	ADCSRA = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADCSRA |= (1 << ADEN);

	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADSC)) > 0);
	return (ADCH);
}


uint16_t adc_RV1_10()
{
	DDRC &= ~(1 << ADC_POT);
	ADMUX = (1 << REFS0);
	ADMUX |= ADC0 ;//ADC_pot = ADC0 = 0
	ADCSRA = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADCSRA |= (1 << ADEN);

	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADSC)) > 0);
	return (ADC);
}

uint16_t adc_LDR_10()
{
	DDRC &= ~(1 << ADC_LDR);
	ADMUX = (1 << REFS0);
	ADMUX |= ADC1 ;
	ADCSRA = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADCSRA |= (1 << ADEN);

	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADSC)) > 0);
	return (ADC);
}

uint16_t adc_NTC_10()
{
	DDRC &= ~(1 << ADC_NTC);
	ADMUX = (1 << REFS0);
	ADMUX |= ADC2 ;
	ADCSRA = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADCSRA |= (1 << ADEN);

	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADSC)) > 0);
	return (ADC);
}
