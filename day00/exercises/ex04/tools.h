#ifndef TOOLS_H
# define TOOLS_H

# include <avr/io.h>
# include <util/delay.h>

typedef enum e_bool
{
	FALSE,
	TRUE
}	e_bool;

typedef enum sw_status
{
	SW_ON,
	SW_OFF
} sw_status;

typedef enum e_pin
{
	D1			=	PB0,
	D2			=	PB1,
	D3			=	PB2,
	MOSI		=	PB3,
	D4			=	PB4,
	SCK			=	PB5,
	X1			=	PB6,
	X2			=	PB7,

	ADC_POT		=	PC0,
	ADC_LDR		=	PC1,
	ADC_NTC		=	PC2,
	RTC_INT		=	PC3,
	SDA			=	PC4,
	SCL			=	PC5,
	RESET		=	PC6,

	RX			=	PD0,
	TX			=	PD1,
	SW1			=	PD2,
	LED_R		=	PD3,
	SW2			=	PD4,
	LED_G		=	PD5,
	LED_B		=	PD6
}	e_pin;

void		set_bit_in_octet(char *oct, int bit_idx, int bit_value);
sw_status	get_sw_status( int regist, e_pin pin);


#endif