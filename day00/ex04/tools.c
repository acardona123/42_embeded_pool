# include "tools.h"

void	set_bit_in_octet(char *oct, int bit_idx, int bit_value)
{
	*oct = ((*oct) & (0xFF ^ (1 << bit_idx))) | (bit_value != 0) << bit_idx;	// modifies *oct to give it 1 if bit_value is not null, 0 otherwise
}

sw_status	get_sw_status( int regist, e_pin pin) //example: get_sw_status(PIND, SW1)
{
	if (regist & (1 << pin))
		return (SW_OFF);
	return (SW_ON);
}
