#include "io_expander.h"

static void	_io_exp_init_in_out_pins();

void	io_exp_init()
{
	_io_exp_init_in_out_pins();
}

static void	_io_exp_init_in_out_pins()
{
	i2c_start((0b0100 << 3) | IO_SWITCH_VALUES, MODE_WRITE);
	i2c_write(IO_CONF_1);
	i2c_write(0b10000000);
	i2c_write(0b00000000);
	i2c_stop();
}

void	io_exp_cmd_D9( bool on)
{
	i2c_start((0b0100 << 3) | IO_SWITCH_VALUES, MODE_WRITE);
	i2c_write(IO_OUT_0);
	i2c_write(0xFF & (~((1 << D9) * on)));
	i2c_stop();
}