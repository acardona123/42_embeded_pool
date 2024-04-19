# include "I2C.h"


# include "UART.h"
void _i2c_ERROR_routine( const char *err_source )
{
	//write the error behaviour here
	uart_printstr("Error in ");
	uart_printstr(err_source);
	uart_printstr(" (");
	uart_printhexa(TW_STATUS);
	uart_printstr(")!\r\n");
	_i2c_stop();
}

void _i2c_init( void )
{
	//schem with all the register and their signification: figure 22-9 p221

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



void	_i2c_stop()
{
	SET(TWCR, TWEN);
	SET(TWCR, TWSTO);
	RESET(TWCR, TWSTA);
	SET(TWCR, TWINT);
}

void	_i2c_start()
{
	// === sending start
	SET(TWCR, TWEN);
	RESET(TWCR, TWSTO);
	SET(TWCR, TWSTA);
	SET(TWCR, TWINT);

	// === waiting for start confirmation
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_START)
		_i2c_ERROR_routine("start");
}

void _i2c_repeated_start()
{
	// === sending restart
	SET(TWCR, TWEN);
	RESET(TWCR, TWSTO);
	SET(TWCR, TWSTA);
	SET(TWCR, TWINT);

	// === waiting for restart confirmation
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_REP_START)
		_i2c_ERROR_routine("repeated_start");
}

void _i2c_send_SLA( char slave_addr, e_mode_RW mode) //master transmitter mode
{

	//=== sending SLA ===

	TWDR = slave_addr << 1; //the slave address
	// RESET(TWAR, TWGCE); //no recognition of the general call address (0x00)
	// if (mode == MODE_READ) //read/write mode
	// 	SET(TWDR, 0);
	// else
	//  	RESET(TWDR, 0);

	TWAMR = 0; //bit 1-7 -> no slave mask, all the slave address is read.

	SET(TWCR, TWINT);


	//=== check SLA reception ===

	while (!(TWCR & (1<<TWINT)))
		;
	if ((mode == MODE_READ && TW_STATUS != TW_MR_SLA_ACK)
		|| (mode == MODE_WRITE && TW_STATUS != TW_MT_SLA_ACK))
		_i2c_ERROR_routine("send_SLA");
	
	SET(TWCR, TWEN);
	RESET(TWCR, TWSTO);
	RESET(TWCR, TWSTA);
}


void _i2c_sending_data_char(char data)
{
	//=== data bit sending
	TWDR = data;
	SET(TWCR, TWEN);
	RESET(TWCR, TWSTO);
	RESET(TWCR, TWSTA);
	SET(TWCR, TWINT);

	//=== data bit transmission confirmation
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MT_DATA_ACK)
		_i2c_ERROR_routine("sending_data");
}

void _i2c_sending_data_str(char *data)
{
	while (*data)
	{
		_i2c_sending_data_char(*data);
		++data;
	}
}

char _i2c_receiving_data( bool stop_after_reception)
{
	char c;

	while (!(TWCR & (1<<TWINT)))
		;
	c = TWDR;
	SET(TWCR, TWEN);
	RESET(TWCR, TWSTO);
	RESET(TWCR, TWSTA);
	if(stop_after_reception)
		_i2c_stop();
	else
		SET(TWCR, TWINT);
	return c;
}