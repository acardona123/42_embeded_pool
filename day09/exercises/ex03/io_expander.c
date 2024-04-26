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
	i2c_start((0b0100 << 3) | IO_SWITCH_VALUES, MODE_WRITE);
	i2c_write(IO_CONF_1);
	i2c_write(0b00000000);
	i2c_stop();
}

void	io_exp_write_in_register(io_cmd_byte target_register, uint8_t register_value)
{
	i2c_start((0b0100 << 3) | IO_SWITCH_VALUES, MODE_WRITE);
	i2c_write(target_register);
	i2c_write(register_value);
	i2c_stop();
}

void	io_exp_cmd_D9( bool on)
{
	io_exp_write_in_register (IO_OUT_0, 0xFF & (~((1 << D9) * on)));
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

void	io_display_digit(uint8_t digit_selector, uint8_t value)
{
	if (digit_selector > 4)
		return;
	io_exp_write_in_register(IO_OUT_0, ~(1 << (3 + digit_selector)));

	digits_pins	segments_value[10] = {IO_zero, IO_one, IO_two, IO_three,
					IO_four, IO_five, IO_six, IO_seven, IO_eight, IO_nine};
	io_exp_write_in_register(IO_OUT_1, segments_value[value % 10]);
}
