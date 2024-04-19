#ifndef MAIN_H
# define MAIN_H

# include "tools.h"
# include "UART.h"
# include "cutie.h"

# define DELETE 127
# define BACKSPACE 8
# define ENTER	'\r'

typedef enum check_phase
{
	CHECK_LOGIN,
	CHECK_PWD,
	VALIDATION,
	CONGRATULATIONS,
	FINISHED
} check_phase;

#endif