#include "main.h"


//16-Bit I/O Expander

typedef struct
{
	uint8_t	value : 3;
}	s_counter;

int main()
{
	s_counter	cpt;
	bool		switch_pressed;

	uart_init();
	i2c_init();
	io_exp_init();
	
	cpt.value = 0;
	io_exp_cmd_out_0(cpt.value << 1);
	switch_pressed = false;
	while (1)
	{
		if (io_exp_sw3_pressed())
		{
			if (!switch_pressed)
			{
				_delay_ms(100);
				switch_pressed = true;
				io_exp_cmd_out_0(++(cpt.value) << 1);
			}
		}
		else
		{
			switch_pressed = false;
		}
		_delay_ms(10);
	}
	return (0);
}