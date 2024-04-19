#ifndef MAIN_H
# define MAIN_H

# include "tools.h"

# ifndef UART_BAUDRATE 
#  define UART_BAUDRATE 115200 //baudrate 115200Hz
# endif

typedef enum check_phase
{
	CHECK_LOGIN,
	CHECK_PWD,
	VALIDATION
} check_phase;

#endif