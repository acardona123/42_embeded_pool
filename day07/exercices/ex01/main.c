#include "main.h"

int main ( void )
{
	DDRD = 0;

	uart_init();

	while(1)
	{
		uart_print_hexa(adc_RV1_8());
		uart_print_str(", ");
		uart_print_hexa(adc_LDR_8());
		uart_print_str(", ");
		uart_print_hexa(adc_NTC_8());
		uart_print_endl();
		_delay_ms(20);
	}
	return (0);
}
