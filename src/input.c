#include "pico/stdlib.h"
#include "input.h"
#include "ui.h"

#define INPUT_SELECT_BTN 16

/* IRQ handler */
bool input_select_pressed(uint gpio, uint32_t events) {
    if (gpio == INPUT_SELECT_BTN && (events & GPIO_IRQ_EDGE_FALL)) {
        spawn_random_dot();
    }
    return true;
}

void inputs_init(void) {
    gpio_init(INPUT_SELECT_BTN);
    gpio_set_dir(INPUT_SELECT_BTN, GPIO_IN);
    gpio_pull_up(INPUT_SELECT_BTN);
    gpio_set_irq_enabled_with_callback(INPUT_SELECT_BTN, GPIO_IRQ_EDGE_FALL, true, (gpio_irq_callback_t) &input_select_pressed);
}