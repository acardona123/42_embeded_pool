#include "main.h"

static void	_display_instructions( void );
void	cpt_init(void);

volatile char			color_code[12] = {};
volatile spi_led_color	color_D6;
volatile spi_led_color	color_D7;
volatile spi_led_color	color_D8;
volatile bool			rainbow_mode;
volatile char			rainbow_pos ;

int main( void )
{
	uint8_t	pos = 0;

	sei();
	uart_init();
	cpt_init();
	spi_master_init();

	color_D6.color_code = 0;
	color_D7.color_code = 0;
	color_D8.color_code = 0;
	rainbow_mode		= false;
	rainbow_pos			= 0;

	uart_print_str_n("=====");
	uart_print_str_n(" START ");
	uart_print_str_n("=====");

	
	_display_instructions();
	spi_led_command(color_D6, color_D7, color_D8);

	while (1);

	return (0);
}


/*
=== INPUT ===
*/

// ... display prompt ...
static void	_display_instructions( void )
{
	uart_print_str_n("Enter a color code following this format:");
	uart_print_str_n("#xxxxxxDn or #RAINBOW , with x in {0-9, A-F}, n being the diode value");
}

// ... getting line ...

  // input validity

static bool	_is_hexa(unsigned char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
}

static input_status	_check_input_is_color_code()
{
	uint8_t idx;

	for (idx = 1; idx < 7; ++idx)
	{
		if (!_is_hexa(color_code[idx]))
			return (INPUT_INVALID);
	}
	if (color_code[idx++] != 'D')
		return (INPUT_INVALID);
	if (color_code[idx] != '6' && color_code[idx] != '7' && color_code[idx] != '8')
		return (INPUT_INVALID);
	return (INPUT_COLOR);
}

static bool	_check_input_is_rainbow()
{
	for (unsigned int i = 1; "#FULLRAINBOW"[i]; ++i)
	{
		if (color_code[i] != "#FULLRAINBOW"[i])
			return (INPUT_INVALID);
	}
	return (INPUT_RAINBOW);
}


static input_status	_check_input_is_valid(uint8_t idx)
{
	if (!idx || *color_code != '#')
		return (INPUT_INVALID);
	if (idx == 9)
		return (_check_input_is_color_code());
	else if (idx == 12)
		return (_check_input_is_rainbow());
	else
		return (INPUT_INVALID);
}


  // actions
	
	//new color

		//str to exa conversion

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

static spi_led_color _interpret_rgb_code()
{
	spi_led_color	color;

	color.brightness = LED_BRIGHTNESS;
	color.red	= _convert_2hex_uint8(color_code + 1);
	color.green	= _convert_2hex_uint8(color_code + 3);
	color.blue	= _convert_2hex_uint8(color_code + 5);
	return (color);
}

void	update_ligth()
{
	spi_led_color	new_color;

	new_color = _interpret_rgb_code();
	if(color_code [8] == '6')
		color_D6 = new_color;
	else if(color_code [8] == '7')
		color_D7 = new_color;
	else
		color_D8 = new_color;
	rainbow_mode = false;
	spi_led_command(color_D6, color_D7, color_D8);
}

		//rainbow
void	do_rainbow()
{
	rainbow_pos = 0;
	rainbow_mode = true;
}
	

void	input_end_line_action(uint8_t idx)
{
	input_status status;

	uart_print_str("\r\n");
	status = _check_input_is_valid(idx);
	switch (status)
	{
		case INPUT_COLOR:
		{
			uart_print_str_n("Light updated\r\n");
			update_ligth();
			break;
		}
		case INPUT_RAINBOW:
		{
			uart_print_str_n("Rainbow mode activated\r\n");
			do_rainbow();
			break;
		}
		default:
		{
			uart_print_str_n("Wrong Format\r\n");
			break;
		}
	}
	_display_instructions();

}

ISR(USART_RX_vect)
{
	char			letter;
	static uint8_t	idx = 0;

	// read a new input
	letter = uart_rx();
	//stop on new line
	if (letter == '\r' || letter == '\n') 
	{
		input_end_line_action(idx);
		idx = 0;
		return ;
	}
	//display letter
	if (letter > ' ')
		uart_tx(letter);
	else
		uart_print_str("[non displayable char]");
	color_code[idx] = letter;
	++idx;
	//check letter validity
	if (idx == 12)
	{
		input_end_line_action(idx);
		idx = 0;
	}
}

/* === RAINBOW ===*/

//triggered by counter overflow

void	cpt_init(void) //timer 1
{
	TCCR1A = 0;
	TCCR1B = 1 << CS10;
	TIMSK1 = 1 << TOIE1;
}

void wheel(uint8_t pos)
{
	spi_led_color	color;

	color.brightness = LED_BRIGHTNESS;
	pos = 255 - pos;
	if (pos < 85)
	{
		color.red	= 255 - pos * 3;
		color.green	= 0;
		color.blue	= pos * 3;
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		color.red	= 0;
		color.green	= pos * 3;
		color.blue = 255 - pos * 3;
	}
	else
	{
		pos = pos - 170;
		color.red	= pos * 3;
		color.green	= 255 - pos * 3;
		color.blue = 0;
	}
	spi_led_command(color, color, color);
}

ISR(TIMER1_OVF_vect)
{
	if (rainbow_mode)
	{
		wheel(rainbow_pos);
	}
	++rainbow_pos;
}
