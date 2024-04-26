#include "io_expander.h"

static void	_io_exp_init_in_out_pins();

void	io_exp_init()
{
	_io_exp_init_in_out_pins();
}

static void	_io_exp_init_in_out_pins()
{
	i2c_start((0b0100 << 3) | IO_SWITCH_VALUES, MODE_WRITE);
	i2c_write(IO_CONF_0);
	i2c_write(0b00000001);
	i2c_stop();
	// i2c_start((0b0100 << 3) | IO_SWITCH_VALUES, MODE_WRITE);
	// i2c_write(IO_CONF_1);
	// i2c_write(0b00000000);
	// i2c_stop();
}

void	io_exp_cmd_out_0(uint8_t register_value)
{
	i2c_start((0b0100 << 3) | IO_SWITCH_VALUES, MODE_WRITE);
	i2c_write(IO_OUT_0);
	i2c_write(0xFF & (~register_value));
	i2c_stop();
}

void	io_exp_cmd_D9( bool on)
{
	io_exp_cmd_out_0 ((1 << D9) * on);
}

bool	io_exp_sw3_pressed(void)
{
	unsigned char	pin_state;

	i2c_start((0b0100 << 3) | IO_SWITCH_VALUES, MODE_WRITE);
	i2c_write(IO_IN_0);
	i2c_start((0b0100 << 3) | IO_SWITCH_VALUES, MODE_READ);
	i2c_read(NACK, &pin_state);
	i2c_stop();

	return (!(pin_state & (1 << SW3)));
}

