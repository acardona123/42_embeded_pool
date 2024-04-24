#ifndef TOOLS_H
# define TOOLS_H

# ifndef __AVR_ATmega328P__
#  define __AVR_ATmega328P__ 1
# endif

# ifndef F_CPU
#  define F_CPU 16000000UL
# endif

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>
# include <stdbool.h>

# define SET(byte, bit_idx) ((byte)|=(1 << (bit_idx)))
# define TOGGLE(byte, bit_idx) ((byte)^=(1 << (bit_idx)))
# define RESET(byte, bit_idx) ((byte)&=~(1<<(bit_idx)))

typedef enum
{
	SUCCESS,
	FAILURE
} e_success;

typedef enum
{
	SW_ON,
	SW_OFF
} sw_status;

typedef enum
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
	LED_R		=	PD3, //blue LED
	SW2			=	PD4,
	LED_G		=	PD5,//red LED
	LED_B		=	PD6 //green LED
}	e_pin;

typedef enum
{
	RGB_R = PD5,
	RGB_G = PD6,
	RGB_B = PD3
}	RGB_lights;

sw_status	get_sw_status( int regist, e_pin pin);


#endif