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
	SW3 = 0,
	D11,
	D10,
	D9,
	DIGGIT_0,
	DIGGIT_2,
	DIGGIT_3,
	DIGGIT_4
}	io_exp_pins_0;

typedef enum
{
	a = 0,
	b,
	c,
	d,
	e,
	f,
	g,
	DPX
}	io_exp_pins_1;

void	io_exp_init();
void	io_exp_cmd_out_0(uint8_t register_value);
void	io_exp_cmd_D9( bool on);
bool	io_exp_sw3_pressed(void);

#endif