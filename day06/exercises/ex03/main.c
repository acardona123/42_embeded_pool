#include "main.h"

static void _routine();

volatile char	color_code[6];
volatile int	idx;
volatile bool	line_validity;
volatile bool	end_line;

int main( void )
{
	uint8_t	pos = 0;

	rgb_init();
	uart_init();

	// uart_print_str_n("=====");
	// uart_print_str_n(" START ");
	// uart_print_str_n("=====");
	
	while (1)
	{
		_routine();
	}

	return (0);
}


// ... display prompt ...
static void	_display_instructions( void )
{
	uart_print_str_n("Enter a color code following this format:");
	uart_print_str_n("#xxxxxx  with x being in {0-9, A-F}");
}

static void _display_response_to_line()
{
	if (line_validity)
		uart_print_str_n(" Ok: the color has been displayed");
	else
		uart_print_str_n(" Wrong format.");
	uart_print_str("\r\n");
}


// ... getting line ...

static void _get_line_and_valid( void )
{
	idx = 0;
	end_line = false;
	line_validity = true;
	while (!end_line) //wait for input wth uart interrupts
	{
	}
	if (idx != 7)
		line_validity = false;
}

ISR(USART_RX_vect)
{
	char	letter;

	// read a new input
	letter = uart_rx();
	//stop on new line
	if (letter == '\r' || letter == '\n') 
	{
		uart_print_str("\r\n");
		end_line = true;
		return ;
	}
	//display letter
	if (letter > ' ')
		uart_tx(letter);
	else
		uart_print_str("[non displayable char]");

	//check letter validity
	if (!line_validity || idx > 6
		|| (idx == 0 && letter != '#')
		|| (idx > 0
			&& !((letter >= '0' && letter <= '9')
				|| (letter >= 'A' && letter <= 'F'))))
	{
		line_validity = false;
		return ;
	}
	else
	{
		if (idx)
			color_code[idx - 1] = letter;
		++idx;
	}
}


// ... str to exa conversion ...

static uint8_t	_convert_hex_uint8_t (char hex_value)
{
	if (hex_value >= 'A' && hex_value <= 'Z')
		return (10 +  hex_value - 'A');
	else if (hex_value >= '0' && hex_value <= '9')
		return (hex_value - '0');
	else
		return (hex_value);
}

static uint8_t _convert_2hex_uint8(volatile char *hex_code)
{
	return (_convert_hex_uint8_t(hex_code[0]) << 4) + _convert_hex_uint8_t(hex_code[1]);	
}

static void _interpret_rgb_code(
				uint8_t *red, uint8_t *green, uint8_t *blue)
{
	*red	= _convert_2hex_uint8(color_code);
	*green	= _convert_2hex_uint8(color_code + 2);
	*blue	= _convert_2hex_uint8(color_code + 4);
}


// ... general function to read a line and process it ...

static void _routine()
{
	_display_instructions();
	_get_line_and_valid();

		// uart_print_str_n("results:");//
	if (line_validity)
	{
		uint8_t red, green, blue;
		_interpret_rgb_code(&red, &green, &blue);
		rgb_set(red, green, blue);
	}
	_display_response_to_line();
}
