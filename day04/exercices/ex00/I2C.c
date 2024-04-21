# include "I2C.h"


# include "UART.h"
void _i2c_ERROR_display( const char *err_source )
{
	//write the error behaviour here
	uart_print_str("Error in ");
	uart_print_str(err_source);
	uart_print_str(" (");
	uart_print_hexa(TW_STATUS);
	uart_print_str(")!\r\n");
}


void _i2c_print_status( void )
{
	uart_print_str("0x");
	uart_print_hexa(TW_STATUS);
	uart_print_str("\r\n");
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
	
	//22.5.2: Bit Rate Generator Unit	( period of SCL )
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
			// any action susceptible to trigger an interrupt will do it by modifying SET(TWCR, TWINT). if set and	when event, TWI Status Register updated + SCL set to low -> stop transmission.
			// to really interrupt the TWI interrupt must be enabled
	
	// 22.6 Using the TWI

		// sei();//to activate interrupts
		RESET(TWCR, TWIE); //enable TWI interrupt. If reset the appli mut poll TWINT Flag to detect an action (TWINT Flag = SET -> waiting for a app response)

	
}

	
int8_t _i2c_start(uint8_t addr, e_mode_RW RW_mode)
{
	// send start
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	// wait for start condition ack
	while (!(TWCR & (1 << TWINT)));
	// check if start is received
	if (TW_STATUS != TW_START)
	{
		_i2c_ERROR_display("start: send_start");
		return 1;
	}
	uart_print_str("Start: ");
	_i2c_print_status();
	// set data with address
	TWDR = (addr << 1) | (RW_mode == MODE_READ);
	// sent data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait the ack
	while (!(TWCR & (1 << TWINT)));
	if (TW_STATUS != TW_MT_SLA_ACK && TW_STATUS != TW_MR_SLA_ACK)
	{
		_i2c_stop();
		_i2c_ERROR_display("start: send SLA");
		return 1;
	}
	uart_print_str("SEND SLA: ");
	_i2c_print_status();
	return 0;
}

void _i2c_stop(void)
{
	// send stop
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	//wait for the stop
    while (TWCR & (1 << TWSTO));
	uart_print_str("STOP: ");
	_i2c_print_status();
}

int8_t _i2c_write(unsigned char data)
{
	// prepare data
	TWDR = data;
	// sent data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait ack
	while ((TWCR & (1 << TWINT)) == 0);
	if (TW_STATUS != TW_MT_DATA_ACK)
	{
		_i2c_ERROR_display("write");
		return (1);
	}
	return (0);
}

uint8_t _i2c_read(uint8_t ack, uint8_t *dest)
{
	// sent receive is ready
	TWCR = (1 << TWINT) | (1 << TWEN) | ((ack == ACK) ? (1 << TWEA) : 0);
	// wait ack
	while (!(TWCR & (1 << TWINT))) {
	}
	if (TW_STATUS != ((ack == ACK) ? TW_MR_DATA_ACK : TW_MR_DATA_NACK))
	{
		_i2c_ERROR_display("write");
		return (1);
	}
	*dest = TWDR;
	return (0);
	// prepare data
}