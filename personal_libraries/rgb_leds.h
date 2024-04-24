#ifndef RGB_LEDS_H
# define RGB_LEDS_H

#include "tools.h"

void rgb_init();
void rgb_set(uint8_t r, uint8_t g, uint8_t b);
void wheel(uint8_t pos);

#endif