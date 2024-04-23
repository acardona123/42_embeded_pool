# include "main.h"

eeprom_status	increment_cpt_idx(int16_t addr, int16_t modulo)
{
	uint8_t 		value;
	eeprom_status	status;

	status = eeprom_read(addr, &value);
	if (status != EEPROM_OK)
	{
		return status;
	}
	++value;
	value %= modulo;
	status = eeprom_write(addr, value);
	return (status);
}

void	set_bit_in_octet(uint8_t *oct, int bit_idx, int bit_value)
{
	*oct = ((*oct) & (0xFF ^ (1 << bit_idx))) | (bit_value != 0) << bit_idx;
}

eeprom_status	display_cpt()
{
	//get the cptvalue
	uint8_t	cpt_idx;
	uint8_t	cpt_value;
	uint8_t	eeprom_status;

	eeprom_read(CPT_IDX_ADDRESS, &cpt_idx);
	switch (cpt_idx)
	{
		case 0:
		{
			eeprom_read(CPT_VALUES_ADDRESS_0, &cpt_value);
			break;
		}
		case 1:
		{
			eeprom_read(CPT_VALUES_ADDRESS_1, &cpt_value);
			break;
		}
		case 2:
		{
			eeprom_read(CPT_VALUES_ADDRESS_2, &cpt_value);
			break;
		}
		case 3:
		{
			eeprom_read(CPT_VALUES_ADDRESS_3, &cpt_value);
			break;
		}
		default :
			break; 
	}
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

eeprom_status reset_eeprom_octet(uint16_t addr, uint8_t value_modulo)
{
	eeprom_status	status;
	uint8_t			value;

	status = eeprom_read(addr, &value);
	if (status != EEPROM_OK)
		return (status);
	if(value > value_modulo)
		status = eeprom_write(addr, 0);
	return (status);
}

int main( void )
{
	sw_status	sw1_status, sw2_status;
	uint8_t		idx_cpt;
	int			*register_del;

	DDRD = 0;								//setting the D register to "listen" all the ports
	DDRB = 0 | (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4); // sets the four DELs pin as outputs

	sw1_status = get_sw_status(PIND, SW1);	//get the current status of switch 1
	sw2_status = get_sw_status(PIND, SW2);	//get the current status of switch 2

	reset_eeprom_octet(CPT_IDX_ADDRESS, 4);
	reset_eeprom_octet(CPT_VALUES_ADDRESS_0, 16);
	reset_eeprom_octet(CPT_VALUES_ADDRESS_1, 16);
	reset_eeprom_octet(CPT_VALUES_ADDRESS_2, 16);
	reset_eeprom_octet(CPT_VALUES_ADDRESS_3, 16);

	if (display_cpt() != EEPROM_OK)
		return 1;

	while (1)
	{
		if (get_sw_status(PIND, SW1) != sw1_status)
		{
			_delay_ms(50);
			sw1_status ^= 1;
			if (sw1_status == SW_ON)
			{
				eeprom_read(CPT_IDX_ADDRESS, &idx_cpt);
				switch (idx_cpt){
					case 0:
					{
						increment_cpt_idx(CPT_VALUES_ADDRESS_0, 16);
						break;
					}
					case 1:
					{
						increment_cpt_idx(CPT_VALUES_ADDRESS_1, 16);
						break;
					}
					case 2:
					{
						increment_cpt_idx(CPT_VALUES_ADDRESS_2, 16);
						break;
					}
					case 3:
					{
						increment_cpt_idx(CPT_VALUES_ADDRESS_3, 16);
						break;
					}
					default :
						continue; 
				}
				display_cpt();
			}
		}
		if (get_sw_status(PIND, SW2) != sw2_status)
		{

			_delay_ms(50);
			sw2_status ^= 1;
			if (sw2_status == SW_ON)
			{
				if (increment_cpt_idx(CPT_IDX_ADDRESS, 4) != EEPROM_OK)
				{
					return (1);
				}
				display_cpt();
			}
		}
	}
	return (0);
}