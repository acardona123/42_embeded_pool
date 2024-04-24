#include "main.h"

int main ( void )
{
	DDRD = 0;

	uart_init();

	while(1)
	{
		uart_print_int(adc_RV1_10());
		uart_print_str(", ");
		uart_print_int(adc_LDR_10());
		uart_print_str(", ");
		uart_print_int(adc_NTC_10());
		uart_print_endl();
		_delay_ms(20);
	}
	return (0);
}
