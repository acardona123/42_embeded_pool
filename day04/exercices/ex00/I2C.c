# include "I2C.h"


# include "UART.h"
void _i2c_ERROR_display( const char *err_source )
{
	//write the error behaviour here
	uart_printstr("Error in ");
	uart_printstr(err_source);
	uart_printstr(" (");
	uart_printhexa(TW_STATUS);
	uart_printstr(")!\r\n");
}

void _i2c_init( void )
{
	//schem with all the register and their signification: figure 22-9 p221

	//enable i2c
	TWCR = 1 << TWEN;

	//22.5.1:SCL and SDA Pins
		// internal pull-ups in the AVR pad
		SET(PORTC, SDA);
		SET(PORTC, SCL);
	
	//22.5.2: Bit Rate Generator Unit  ( period of SCL )
		// SCL_freq = CPU_clock_freq / [16 + 2*(TWBR)*(Prescaler_value)] need to be > 16*CPU_clock_freq 
		//			<=> TWBR = (CPU_clock_freq / SCL_freq- 16)/(2 * Prescale)
		int	presclaler = 1; //-> pour que TWBR < 256 -> prescale de 1 (cf Table 22-7. TWI Bit Rate Prescaler)
		TWBR = (F_CPU / I2C_BIT_RATE_FREQUENCY - 16) / (2 * presclaler);

		// TWI Bit Rate Register (TWBR) 

		// Prescaler bits in the TWI Status Register (TWSR) ( Table 22-7 on page 241.)

	// 22.5.3 Bus interface unit 
		//nothing to do ?

	// 22.5.4 Address match unit
		// trigger if all bytes must be compared to known address
		RESET(TWAR, TWGCE);

	// 22.5.4 Control unit
		//TWI interrupt (list of interrupt in this paragraph)
			// any action susceptible to trigger an interrupt will do it by modifying SET(TWCR, TWINT). if set and  when event, TWI Status Register updated + SCL set to low -> stop transmission.
			// to really interrupt the TWI interrupt must be enabled
	
	// 22.6 Using the TWI

		// sei();//to activate interrupts
		RESET(TWCR, TWIE); //enable TWI interrupt. If reset the appli mut poll TWINT Flag to detect an action (TWINT Flag = SET -> waiting for a app response)

	
}

void	_i2c_wait( void )
{
	while (!(TWCR & (1<<TWINT)))
		;
}

int8_t	_i2c_stop()
{
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
	while (TWCR & (1 << TWSTO))
		;
	return (0);
}

int8_t	_i2c_start( void )
{
	// === sending start
	TWCR = (1 << TWEN) | (1 << TWSTA ) | (1 << TWINT);

	_i2c_wait();
	if (TW_STATUS != TW_START)
	{
		_i2c_ERROR_display("start");
		return (1);
	}
	return (0);
}

int8_t _i2c_repeated_start()
{
	TWCR = (1 << TWEN)| (1 << TWSTA ) | (1 << TWINT);
	_i2c_wait();
	if (TW_STATUS != TW_REP_START)
	{
		_i2c_ERROR_display("start");
		return (1);
	}
	return (0);
}

int8_t _i2c_send_SLA( char slave_addr, e_mode_RW mode) //master transmitter mode
{

	//=== sending SLA ===
	TWDR = (slave_addr << 1) + (mode == MODE_READ); //the slave address + read/write mode
	// TWAMR = 0; //bit 1-7 -> no slave mask, all the slave address is read.

	SET(TWCR, TWINT);
	//=== check SLA reception ===

	_i2c_wait();
	if (TW_STATUS != ( (mode == MODE_READ) ? TW_MR_SLA_ACK : TW_MT_SLA_ACK))
	{
		_i2c_ERROR_display("start");
		return (1);
	}
	return (0);
}


int8_t _i2c_sending_data_char(char data)
{
	//=== data bit sending
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);

	//=== data bit transmission confirmation
	_i2c_wait();
	if (TW_STATUS != TW_MT_DATA_ACK)
	{
		_i2c_ERROR_display("start");
		return (1);
	}
	return (0);
}

int8_t _i2c_sending_data_str(char *data, long unsigned  len)
{
	for (long unsigned int i = 0; i < len; ++i)
	{
		if (_i2c_sending_data_char(data[i]))
			return (1);
	}
	return (0);
}

char _i2c_receiving_data( bool stop_after_reception)
{
	char c;

	while (!(TWCR & (1<<TWINT)))
		;
	c = TWDR;
	if(stop_after_reception)
	{
		TWCR = (1 << TWEN);
		_i2c_stop();
	}
	else
	{
		TWCR = (1 << TWEN) | (1 << TWINT);
	}
	SET(TWCR, TWINT);
	return c;
}