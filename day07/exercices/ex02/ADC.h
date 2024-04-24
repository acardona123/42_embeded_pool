#ifndef ADC_H
# define ADC_H

# include "tools.h"

typedef enum
{
	ADC0	= 0b0000,
	ADC1	= 0b0001,
	ADC2	= 0b0010,
	ADC3	= 0b0011,
	ADC4	= 0b0100,
	ADC5	= 0b0101,
	ADC6	= 0b0110,
	ADC7	= 0b0111,
	ADC8	= 0b1000,
	Vbg		= 0b1110,
	GND		= 0b1111
} adc_channel;

unsigned char	adc_RV1_8();
unsigned char	adc_LDR_8();
unsigned char	adc_NTC_8();
uint16_t		adc_RV1_10();
uint16_t		adc_LDR_10();
uint16_t		adc_NTC_10();


#endif