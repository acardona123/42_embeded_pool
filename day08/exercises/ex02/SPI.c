#include "SPI.h"

//SPI – Serial Peripheral Interface
//ATMega328P, doc chapter 19

// my functions:
void spi_master_init(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (1 << SPI_MOSI)|(1 << SPI_SCK) | (1 << SPI_SS); // /!\ the SS MUST be set,otherwise there is interferences and the SPI_MASTER will not stay SET
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR =	SPI_ENABLE
			| SPI_INTERRUPT_ENABLE
			| ! SPI_DATA_ORDER_INVERT
			| SPI_IS_MASTER
			| ! SPI_CLOCK_POLARITY_FALLING
			| ! SPI_CLOCK_PHASE_SAMPLE_ON_TRAILING_EDGE
			//prescaler: need a frequency in [800; 1200] kHz -> prescaler in [20; 13] -> prescaler = 16
			| 1 * (1 << SPR0) | 0 * (1 << SPR0);

}

void spi_master_transmit(char cData)
{
	//no address given here fot the DEL, they are supposed to be the only SPI slave in this board

	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}


/*
====
Commanding the DELs
====
*/

spi_led_color spi_led_generate_colors(uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue)
{
	spi_led_color color_code;

	color_code.brightness = brightness & 0b11111;
	color_code.red = red;
	color_code.green = green;
	color_code.blue = blue;
	return (color_code);
}

static void _spi_start_stop_signal(bool is_stop)
{
	//32 * is_stop to start/stop
	spi_master_transmit(is_stop * 0xFF);
	spi_master_transmit(is_stop * 0xFF);
	spi_master_transmit(is_stop * 0xFF);
	spi_master_transmit(is_stop * 0xFF);
}

static void _spi_led_frame_one_led(spi_led_color led_data)
{
	spi_master_transmit(led_data.brightness | 0b11100000);
	spi_master_transmit(led_data.blue);
	spi_master_transmit(led_data.green);
	spi_master_transmit(led_data.red);
}

void	spi_led_command(spi_led_color D6, spi_led_color D7, spi_led_color D8)
{
	//32 * 0 to start
	_spi_start_stop_signal(false);

	_spi_led_frame_one_led(D6);
	_spi_led_frame_one_led(D7);
	_spi_led_frame_one_led(D8);

	//32 * 1 to stop
	_spi_start_stop_signal(true);
}