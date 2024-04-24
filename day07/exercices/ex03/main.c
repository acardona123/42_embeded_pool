#include "main.h"

int main ( void )
{
	DDRD = 0;

	uart_init();

	while(1)
	{
		uart_print_int(adc_INT_10());
		uart_print_endl();
		_delay_ms(20);
	}
	return (0);
}
