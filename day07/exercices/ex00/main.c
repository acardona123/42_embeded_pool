#include "main.h"



int main ( void )
{
	unsigned char c;

	DDRD = 0;

	uart_init();
	// adc_init();

	while(1)
	{
		c = adc_RV1_8();
		uart_print_hexa(c);
		uart_print_endl();
		_delay_ms(20);
	}
	
	return (0);
}
