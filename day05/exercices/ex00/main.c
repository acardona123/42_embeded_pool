# include "main.h"

eeprom_status	increment_cpt()
{
	uint8_t 		cpt_value;
	eeprom_status	status;

	status = eeprom_read(CPT_ADDRESS, &cpt_value);
	if ( status != EEPROM_OK)
		return status;
	return (eeprom_write(CPT_ADDRESS, (++cpt_value) % 16));
}

eeprom_status	decrement_cpt()
{
	uint8_t 		cpt_value;
	eeprom_status	status;

	status = eeprom_read(CPT_ADDRESS, &cpt_value);
	if ( status != EEPROM_OK)
		return status;
	return (eeprom_write(CPT_ADDRESS, (--cpt_value) % 16));
}

void	set_bit_in_octet(uint8_t *oct, int bit_idx, int bit_value)
{
	*oct = ((*oct) & (0xFF ^ (1 << bit_idx))) | (bit_value != 0) << bit_idx;
}


eeprom_status	display_cpt()
{
	//get the cptvalue
	uint8_t	cpt_value;
	uint8_t	eeprom_status;
	eeprom_status = eeprom_read(CPT_ADDRESS, &cpt_value);
	if ( eeprom_status != EEPROM_OK)
		return (eeprom_status);

	//gets the current value of the register B to avoid erasing the values of the bits that won't be modified by a light command
	uint8_t current_register;
	current_register = PORTB;
	//sets the bits allocated to the del to encode the counter value
	set_bit_in_octet(&current_register, D1, cpt_value & (1 << 0));
	set_bit_in_octet(&current_register, D2, cpt_value & (1 << 1));
	set_bit_in_octet(&current_register, D3, cpt_value & (1 << 2));
	set_bit_in_octet(&current_register, D4, cpt_value & (1 << 3));
	PORTB=current_register;

	return (EEPROM_OK);
}

int main( void )
{
	sw_status	sw1_status, sw2_status;
	int			*register_del;

	DDRD = 0;								//setting the D register to "listen" all the ports
	sw1_status = get_sw_status(PIND, SW1);	//get the current status of switch 1
	sw2_status = get_sw_status(PIND, SW2);	//get the current status of switch 2
	
	DDRB = 0 | (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4); // sets the four DELs pin as outputs

	uint8_t		cpt_read;
	if(eeprom_read(CPT_ADDRESS, &cpt_read) != EEPROM_OK)
		return 1;

	while (1)
	{
		if (get_sw_status(PIND, SW1) != sw1_status)
		{
			_delay_ms(50);
			sw1_status ^= 1;
			if (sw1_status == SW_ON
				&& (increment_cpt() != EEPROM_OK || display_cpt() != EEPROM_OK))
			{
					return 1;
			}
		}
		if (get_sw_status(PIND, SW2) != sw2_status)
		{

			_delay_ms(50);
			sw2_status ^= 1;
			if (sw2_status == SW_ON
				&& (decrement_cpt() != EEPROM_OK || display_cpt() != EEPROM_OK))
			{
					return (1);
			}
		}
	}
	return (0);
}