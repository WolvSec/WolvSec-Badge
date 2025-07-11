#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include "lvgl.h"
#include "pico/stdlib.h"

typedef struct {
  lv_display_t *disp;
  lv_color_t *buf1;
  lv_color_t *buf2;
  repeating_timer_t lvgl_timer;
} lcd_driver_t;

int lcd_driver_init(lcd_driver_t *driver);
lv_display_t *lcd_driver_get_display(lcd_driver_t *driver);

#endif
