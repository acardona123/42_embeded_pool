#include "UART.h"

void init_USART( void )
{
	// asynchronous USART (doc table 20-8)
		RESET(UCSR0C, UMSEL00);
		RESET(UCSR0C, UMSEL01);
	
	// set baudrate to 115200Hz
		// using USART Baud Rate Register (UBRRn). cf table 20-1 in normal mode.
		// UBRR0 = fosc/(16*BAUD)-1  fosc should be 13MHz according to the shema.pdf, but the subject indicates to use F_CPU
		int	ubrr0 = (float)(F_CPU) / (16 * UART_BAUDRATE) - 1; // ~= 7.68 ~= 8 could have been bigger then  255 -> UBRR0 is saved in two bytes (high and low), UBBROH and UBBROL
		++ubrr0; //to round it to 8
	UBRR0L = (unsigned  char)(ubrr0 & 0xFF);
	UBRR0H = (unsigned  char)(ubrr0 >> 8);
		
	// set the frame format (if the pattern followed in a msg tram),  doc 20.4
		// format 8N1 requiered by the subject. Means 8bit data, no parity bit, 1 stop bit. It is the default format
			//number of bits (doc table 20-11)
				SET(UCSR0C,UCSZ00);
				SET(UCSR0C,UCSZ01);
				RESET(UCSR0B,UCSZ02);
			//parity mode: (doc table 20-9)
				RESET(UCSR0C,UPM00);
				RESET(UCSR0C,UPM01);
			//stop bit: (doc table 20-10)
				RESET(UCSR0C,USBS0);

	// enabling Transmitter (not receiver)
		SET(UCSR0B, TXEN0); //transmission on
		SET(UCSR0B, RXEN0); //transmission off

	// enabling interrupt , doc 20.11.3
		SET(UCSR0B, RXCIE0); //reception
		RESET(UCSR0B, TXCIE0); //transmission
}

// ISR(USART_RX_vect)
// {
// 	unsigned char	c;
// 	cli();
// 	c = uart_rx();
// 	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
// 		c ^= 1 << 5;
// 	uart_tx(c);
// 	sei();
// }

//from the doc 20.6.1
//to transmit a char (8bits) one must write the char 
void uart_tx( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1 << UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

//from the doc 20.7.1
//to transmit a char (8bits) one must write the char 
unsigned char uart_rx( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1 << RXC0)) );
	/* Get and return received data from buffer */
	return (UDR0);
}

void uart_print_str( const char *str )
{
	unsigned long long int i;


	i = 0;
	while (str[i])
	{
		uart_tx(str[i]);
		++i;
	}
	uart_tx(0);
}

void	uart_txt_erase_char( void )
{
	uart_tx(8);
	uart_tx(' ');
	uart_tx(8);
}

void	uart_print_hexa( unsigned char c )
{
	uart_print_str("0x");
	uart_tx("0123456789ABCDEF"[c >> 4]);
	uart_tx("0123456789ABCDEF"[c & 0x0f]);
}

void	uart_print_int( int n )
{
	unsigned int un;

	if (n < 0)
	{
		uart_tx('-');
		un = -n;
	}
	else
	{
		un = n;
	}
	if (un >= 10)
	{
		uart_print_int(un / 10);
	}
	uart_tx(un % 10 + '0');
}

void	uart_print_float( float f, unsigned int precision)
{
	int n;

	n = (int)f;
	uart_print_int(n);
	uart_tx(',');
	uart_print_int((int)((f - n) * 10.f * precision + 0.5f));
}