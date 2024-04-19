#ifndef UART_H
# define UART_H

#include "tools.h"

# ifndef UART_BAUDRATE 
#  define UART_BAUDRATE 115200 //baudrate 115200Hz
# endif

void			init_USART( void );
void			uart_tx( unsigned char data );
void			uart_printstr( const char*str );
void			uart_printhexa( unsigned char c );
unsigned char	uart_rx( void );
void			uart_txt_erase_char( void );

#endif