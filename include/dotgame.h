#ifndef DOTGAME_H
#define DOTGAME_H
#include <pico/types.h>
#include <stdbool.h>
#include <stdint.h>

void dotgame_inputs_init(void);
void dotgame_init(uint gpio, uint32_t events);
void dot_updatex(uint gpio);
void dot_updatey(uint gpio);

#endif