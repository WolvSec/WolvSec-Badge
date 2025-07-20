/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "lcd_driver.h"
#include "pico/stdlib.h"
#include "ui.h"
#include "input.h"
#include <stdio.h> /* For debug */

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
  // A device like Pico that uses a GPIO for the LED will define
  // PICO_DEFAULT_LED_PIN so we can use normal GPIO functionality to turn the
  // led on and off
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
  // For Pico W devices we need to initialise the driver etc
  // Graphics section:
  return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
  // Just set the GPIO on or off
  gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
  // Ask the wifi "driver" to set the GPIO on or off
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

lcd_driver_t lcd;

int main() {
  stdio_init_all();
  lv_init();

  lcd_driver_init(&lcd);
  inputs_init();
  ui_display_cat();
  // ui_lv_example_get_started_2();

  while (true) {
    lv_timer_handler();
    sleep_ms(5);
  }
}