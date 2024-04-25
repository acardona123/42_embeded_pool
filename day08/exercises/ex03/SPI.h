#ifndef SPI_H
# define SPI_H

#include "tools.h"

#define SPI_INTERRUPT_ENABLE					(1 << SPIE)
#define SPI_ENABLE								(1 << SPE)
#define SPI_DATA_ORDER_INVERT					(1 << DORD)
#define SPI_IS_MASTER							(1 << MSTR)
#define SPI_CLOCK_POLARITY_FALLING				(1 << CPOL)
#define SPI_CLOCK_PHASE_SAMPLE_ON_TRAILING_EDGE	(1 << CPHA)

typedef enum
{
	SPI_SS		=	PB2,
	SPI_MOSI	=	PB3,
	SPI_MISO	=	PB4,
	SPI_SCK		=	PB5
} SPI_pins;

typedef union
{
	uint32_t color_code;
	struct
	{
		uint8_t	brightness;
		uint8_t	blue;
		uint8_t	green;
		uint8_t	red;
	};
} spi_led_color;


void 			spi_master_init(void);
void 			spi_master_transmit(char cData);

spi_led_color	spi_led_generate_colors(uint8_t brightness,
					uint8_t red, uint8_t green, uint8_t blue);
void			spi_led_command(spi_led_color D6, spi_led_color D7, spi_led_color D8);

#endif