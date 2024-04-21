#ifndef TOOLS_H
# define TOOLS_H

# define __AVR_ATmega328P__ 1

# ifndef F_CPU
#  define F_CPU 16000000UL
# endif

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>
# include <stdbool.h>

# define SET(x, y) ((x)|=(1 << (y)))
# define TOGGLE(x, y) ((x)^=(1 << (y)))
# define RESET(x, y) ((x)&=~(1<<(y)))

typedef enum e_success
{
	SUCCESS,
	FAILURE
} e_success;

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

sw_status	get_sw_status( int regist, e_pin pin);


#endif