#ifndef IO_EXPANDER_H
# define IO_EXPANDER_H

#include "tools.h"
#include "I2C.h"
#include "UART.h"

#define IO_SWITCH_VALUES 0b000 //the values of the switches A0, A1, A2 on the ship (hardware)

typedef enum
{
	IO_IN_0 = 0,
	IO_IN_1,
	IO_OUT_0,
	IO_OUT_1,
	IO_POL_0,
	IO_POL_1,
	IO_CONF_0,
	IO_CONF_1
}	io_cmd_byte;

typedef enum
{
	SW3 	= 0,
	D11 	= 1 << 0,
	D10 	= 1 << 1,
	D9 		= 1 << 2,
	DIGIT_1 = 1 << 3,
	DIGIT_2 = 1 << 4,
	DIGIT_3 = 1 << 5,
	DIGIT_4 = 1 << 6,
}	io_exp_pins_0;

typedef enum
{
	IO_zero		= 0b00111111,
	IO_one		= 0b00000110,
	IO_two		= 0b01011011,
	IO_three	= 0b01001111,
	IO_four		= 0b01100110,
	IO_five		= 0b01101101,
	IO_six		= 0b01111101,
	IO_seven	= 0b00000111,
	IO_eight	= 0b01111111,
	IO_nine		= 0b01101111,
	IO_DP		= 0b10000000
}	digits_pins;


void	io_exp_init();
bool	io_exp_sw3_pressed(void);
void	io_exp_write_in_register(io_cmd_byte target_register, uint8_t register_value);
void	io_exp_cmd_D9( bool on);
void	io_display_digit(uint8_t digit_selector, uint8_t value);

#endif