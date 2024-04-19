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


void			init_USART( void );


volatile const char		g_login[] = "login";
volatile const char		g_pwd[]	= "pwd";
volatile unsigned int	g_login_cpt_since_error;
volatile unsigned int	g_login_idx;
volatile unsigned int	g_pwd_cpt_since_error;
volatile unsigned int	g_pwd_idx;
volatile check_phase	g_checking;

void 							reset_session( void );
static unsigned long long int	ft_strlen( volatile const char *str);


int main()
{
	SET(DDRB, D1);

	init_USART();
	reset_session();
	sei();

	while (1)
	{
		if (g_checking == CONGRATULATIONS)
		{
			cli();
			display_film();
			display_ok();

			g_checking = FINISHED;
		}
	}
	return (0);
}


void reset_session( void )
{
	g_login_cpt_since_error = 0;
	g_pwd_cpt_since_error = 0;
	g_login_idx = 0;
	g_pwd_idx = 0;
	g_checking = CHECK_LOGIN;
	uart_printstr("Enter your login:\n\r");
	uart_printstr("\tusername: ");
}

void	check_value(unsigned char c, volatile const char *reference_str,
			volatile unsigned int *idx, volatile unsigned *error_cpt)
{
	if (c == '\0')
	{
		*idx = 0;
		return;
	}
	if (c == ENTER)
	{
		++g_checking;
		if (g_checking == CHECK_PWD)
			uart_printstr("\n\r\tpassword: ");
		return;
	}
	if (c == DELETE)
	{
		if (*idx)
		{
			--(*idx);
			if (*error_cpt)
				--(*error_cpt);
			uart_txt_erase_char();
		}
		return;
	}
	if (*error_cpt || *idx > ft_strlen(reference_str) || c != reference_str[*idx])
	{
		++(*error_cpt);
	}
	++(*idx);
}

void	yahoooooooooooooo( void )
{
	uart_printstr("Congratulation ");
	uart_printstr((const char *
	)g_login);
	uart_printstr(", I love you so much ! 🙂\n\r");
	g_checking = CONGRATULATIONS;
}

void	looooooooooooser( void )
{
	uart_printstr("Bad combination username/password\r\n\n");
	reset_session();
}

static unsigned long long int ft_strlen( volatile const char *str)
{
	unsigned long long i= 0;
	while (str[i])
		++i;
	return (i);
}

ISR(USART_RX_vect)
{
	unsigned char	c;
 
	cli();
	c = uart_rx();
	if (g_checking == CHECK_LOGIN)
	{
		if (c != DELETE && c != ENTER)
			uart_tx(c);
		check_value(c, g_login, &g_login_idx, &g_login_cpt_since_error);
	}
	else
	{
		if (c != DELETE && c != ENTER)
			uart_tx('*');
		check_value(c, g_pwd, &g_pwd_idx, &g_pwd_cpt_since_error);
		if (g_checking == VALIDATION)
		{
			uart_printstr("\r\n");
			if (!g_login_cpt_since_error && !g_pwd_cpt_since_error)
			{
				yahoooooooooooooo();
				return;
			}
			else
				looooooooooooser();
		}
	}
	sei();
}

