#ifndef UART_H
# define UART_H

#include "tools.h"

# ifndef UART_BAUDRATE 
#  define UART_BAUDRATE 115200 //baudrate 115200Hz
# endif

void			uart_init( void );

void			uart_tx( unsigned char data );
void			uart_print_str( const char*str );
void			uart_print_str_n( const char *str );
void			uart_print_hexa( unsigned char c );

unsigned char	uart_rx( void );

void			uart_txt_erase_char( void );

#endif