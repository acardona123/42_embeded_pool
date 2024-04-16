# include "main.h"



void	increment_semi_octet(char *cpt)
{
	if (*cpt == (1 << 4))
		*cpt = 0;
	else
		++(*cpt);
}
void	decrement_semi_octet(char *cpt)
{
	if (cpt == 0)
		*cpt = 1 << 4;
	else
		--(*cpt);

}



char	display_semi_octet(char cpt_value)
{
	char current_register;

	//gets the current value of the register B to avoid erasing the values of the bits that won't be modified by a light command
	current_register = PORTB;

	//sets the bits allocated to the del to encode the counter value
	set_bit_in_octet(&current_register, D1, cpt_value & (1 << 0));
	set_bit_in_octet(&current_register, D2, cpt_value & (1 << 1));
	set_bit_in_octet(&current_register, D3, cpt_value & (1 << 2));
	set_bit_in_octet(&current_register, D4, cpt_value & (1 << 3));
	PORTB=current_register;

	return (current_register);
}

int main( void )
{
	char		cpt;
	sw_status	sw1_status, sw2_status;
	int			*register_del;

	cpt = 0;


	DDRD = 0;								//setting the D register to "listen" all the ports
	sw1_status = get_sw_status(PIND, SW1);	//get the current status of switch 1
	sw2_status = get_sw_status(PIND, SW2);	//get the current status of switch 2
	
	DDRB = 0 | (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4); // sets the four DELs pin as outputs

	while (1)
	{
		if (get_sw_status(PIND, SW1) != sw1_status)
		{
			_delay_ms(50);
			sw1_status ^= 1;
			if (sw1_status == SW_ON)
			{
				increment_semi_octet(&cpt);
				display_semi_octet(cpt);
			}
		}
		if (get_sw_status(PIND, SW2) != sw2_status)
		{
			_delay_ms(50);
			sw2_status ^= 1;
			if (sw2_status == SW_ON)
			{
				decrement_semi_octet(&cpt);
				display_semi_octet(cpt);
			}
		}
	}
	return (0);
}