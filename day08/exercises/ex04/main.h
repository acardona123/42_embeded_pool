#ifndef MAIN_H
# define MAIN_H

# include "tools.h"
# include "SPI.h"
# include "UART.h"

#define LED_BRIGHTNESS 1

typedef enum
{
	INPUT_INVALID,
	INPUT_RAINBOW,
	INPUT_COLOR
}	input_status;


#endif