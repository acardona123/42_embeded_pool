# include "main.h"

// http://www.rjhcoding.com/avrc-uart.php
// doc for ATmega48A/PA/88A/PA/168A/PA/328/P

/* USART initialization: (doc 20.5)
The USART has to be initialized before any communication can take place. The initialization process normally
consists of setting the baud rate, setting frame format and enabling the Transmitter or the Receiver depending
on the usage. For interrupt driven USART operation, the Global Interrupt Flag should be cleared (and interrupts
globally disabled) when doing the initialization
*/

//to receive on the terminal:
//	screen /dev/ttyUSB0 115200
// to exit: ctrl+a puis k puis y

void	init_USART( void );
void	cpt_init(void);
void	uart_tx( unsigned char data );

int main()
{
	init_USART();
	cpt_init();
	sei();

	while (1);
	return (0);
}


void init_USART( void )
{
	// asynchronous USART (doc table 20-8)
		RESET(UCSR0C, UMSEL00);
		RESET(UCSR0C, UMSEL01);
	
	// set baudrate to 115200Hz
		// using USART Baud Rate Register (UBRRn). cf table 20-1 in normal mode.
		// UBRR0 = fosc/(16*BAUD)-1  fosc should be 13MHz according to the shema.pdf, but the subject indicates to use F_CPU
		int	ubrr0 = (float)(F_CPU) / (16 * UART_BAUDRATE) - 1; // ~= 7.68 ~= 8 could have been bigger then  255 -> UBRR0 is saved in two bytes (high and low), UBBROH and UBBROL
		++ubrr0; //to round it to 8 -> see table 20-7
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
		RESET(UCSR0B, RXEN0); //reception off
}

//from the doc 20.6.1
//to transmit a char (8bits) one must write the char 
void uart_tx( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void	cpt_init(void)
{
	//predivider // divide by 256
	RESET(TCCR1B, CS10);
	RESET(TCCR1B, CS11);
	SET(TCCR1B, CS12); 

	//set comparisons doc Table 16-1 
	RESET(TCCR1A, COM1A0); //(no port operation)
	RESET(TCCR1A, COM1A1);
	OCR1A = 62499; // formula end of 16.9.2

	//wave generation mode bit description
	RESET(TCCR1A, WGM10);
	RESET(TCCR1A, WGM11);
	SET(TCCR1B, WGM12);
	RESET(TCCR1B, WGM13);

	//set interrupt doc 16.11.8
	SET(TIMSK1, OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	uart_tx('z');
}