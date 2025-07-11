#include "lcd_driver.h"
#include "lcd_hal.h"
#include <stdio.h>

#define LCD_RES_X 240
#define LCD_RES_Y LCD_RES_X

static int32_t send_cmd_cb(lv_display_t *disp, const uint8_t *cmd,
                           size_t cmd_size, const uint8_t *param,
                           size_t param_size) {
  lcd_hal_send_cmd(cmd, cmd_size);
  if (param && param_size > 0) {
    lcd_hal_send_data(param, param_size);
  }
  lv_display_flush_ready(disp);
  return 0;
}

static int32_t send_color_cb(lv_display_t *disp, const uint8_t *cmd,
                             size_t cmd_size, uint8_t *param,
                             size_t param_size) {
  lcd_hal_send_cmd(cmd, cmd_size);
  if (param && param_size > 0) {
    lcd_hal_send_data_dma(param, param_size,
                          (void (*)(void *))lv_display_flush_ready, disp);
  }
  return 0;
}

static bool repeating_lvgl_timer_callback(struct repeating_timer *t) {
  lv_tick_inc(5);
  return true;
}

int lcd_driver_init(lcd_driver_t *driver) {
  uint32_t buf_size;
  lcd_hal_init();
  add_repeating_timer_ms(5, repeating_lvgl_timer_callback, NULL,
                         &(driver->lvgl_timer));
  driver->disp =
      lv_st7789_create(240, 240, 0, (lv_st7789_send_cmd_cb_t)send_cmd_cb,
                       (lv_st7789_send_color_cb_t)send_color_cb);
  buf_size =
      LCD_RES_X * LCD_RES_Y / 10 *
      lv_color_format_get_size(lv_display_get_color_format(driver->disp));
  driver->buf1 = lv_malloc(buf_size);
  if (driver->buf1 == NULL) {
    printf("display buffer lv_malloc failed\n");
    return -1;
  }

  driver->buf2 = lv_malloc(buf_size);
  if (driver->buf2 == NULL) {
    printf("display buffer lv_malloc failed\n");
    lv_free(driver->buf1);
    return -1;
  }
  lv_display_set_buffers(driver->disp, driver->buf1, driver->buf2, buf_size,
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_st7789_set_invert(driver->disp, 1);
  return 0;
}

lv_display_t *lcd_driver_get_display(lcd_driver_t *driver) {
  return driver->disp;
}