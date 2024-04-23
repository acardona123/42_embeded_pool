# include "tools.h"

sw_status	get_sw_status( int regist, e_pin pin) //example: get_sw_status(PIND, SW1)
{
	if (regist & (1 << pin))
		return (SW_OFF);
	return (SW_ON);
}
