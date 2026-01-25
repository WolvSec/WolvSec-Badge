/* Simple game toy example */
#include "include/dotgame.h"
#include "pico/stdlib.h"
#include "ui.h"
#include <hardware/gpio.h>
#include <hardware/irq.h>
#include <hardware/regs/intctrl.h>
#include <pico/time.h>
#include <src/core/lv_obj.h>
#include <src/core/lv_obj_pos.h>
#include <src/core/lv_obj_style_gen.h>
#include <src/display/lv_display.h>
#include <src/draw/lv_draw_rect.h>
#include <src/misc/lv_area.h>
#include <src/misc/lv_palette.h>
#include <src/misc/lv_style.h>
#include <src/stdlib/lv_mem.h>
#include <src/stdlib/lv_string.h>
#include <src/widgets/label/lv_label.h>
#include <stdio.h>
#include <string.h>

#define BIT(n) (1U << (n))

#define INPUT_SELECT_BTN 16
#define INPUT_RESET_BTN 7
#define INPUT_UP_BTN 8
#define INPUT_DOWN_BTN 9
#define INPUT_LEFT_BTN 10
#define INPUT_RIGHT_BTN 11

#define DX 5
#define DY 5

static lv_obj_t *dot_player = NULL;

void dotgame_inputs_init(void) {
  uint32_t gpio_mask = BIT(INPUT_SELECT_BTN) | BIT(INPUT_RESET_BTN) |
                       BIT(INPUT_UP_BTN) | BIT(INPUT_DOWN_BTN) |
                       BIT(INPUT_LEFT_BTN) | BIT(INPUT_RIGHT_BTN);
  gpio_init_mask(gpio_mask);
  gpio_set_dir_in_masked(gpio_mask);
  gpio_pull_up(INPUT_SELECT_BTN);
  gpio_pull_up(INPUT_RESET_BTN);
  gpio_pull_up(INPUT_DOWN_BTN);
  gpio_pull_up(INPUT_UP_BTN);
  gpio_pull_up(INPUT_LEFT_BTN);
  gpio_pull_up(INPUT_RIGHT_BTN);
  gpio_set_irq_enabled_with_callback(INPUT_SELECT_BTN, GPIO_IRQ_EDGE_FALL, true,
                                     (gpio_irq_callback_t)&dotgame_init);
  gpio_set_irq_enabled(INPUT_RESET_BTN, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(INPUT_UP_BTN, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(INPUT_DOWN_BTN, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(INPUT_LEFT_BTN, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(INPUT_RIGHT_BTN, GPIO_IRQ_EDGE_FALL, true);

  return;
}

void dotgame_init(uint gpio, uint32_t events) {
  switch (gpio) {
  case INPUT_UP_BTN:
    dot_updatey(INPUT_UP_BTN);
    return;
  case INPUT_DOWN_BTN:
    dot_updatey(INPUT_DOWN_BTN);
    return;
  case INPUT_LEFT_BTN:
    dot_updatex(INPUT_LEFT_BTN);
    return;
  case INPUT_RIGHT_BTN:
    dot_updatex(INPUT_RIGHT_BTN);
    return;
  default:
    break;
  }
  if (dot_player != NULL) {
    lv_obj_delete(dot_player);
  }
  lv_obj_clean(lv_screen_active());
  lv_obj_set_style_bg_color(lv_screen_active(),
                            lv_palette_main(LV_PALETTE_GREY), 0);
  dot_player = lv_obj_create(lv_screen_active());
  lv_obj_set_style_bg_color(dot_player, lv_palette_main(LV_PALETTE_DEEP_ORANGE),
                            0);
  lv_obj_set_style_radius(dot_player, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_size(dot_player, 20, 20);
  // lv_obj_align(dot_player, LV_ALIGN_CENTER, 0, 0);
}

void dot_updatex(uint gpio) {
  if (NULL == dot_player) {
    return;
  }
  lv_obj_update_layout(dot_player);
  int x = lv_obj_get_x(dot_player);
  int y = lv_obj_get_y(dot_player);
  if (gpio == INPUT_RIGHT_BTN) {
    lv_obj_set_pos(dot_player, x + DX, y);
  } else {
    lv_obj_set_pos(dot_player, x - DX, y);
  }
}

void dot_updatey(uint gpio) {
  if (NULL == dot_player) {
    return;
  }
  lv_obj_update_layout(dot_player);
  int x = lv_obj_get_x(dot_player);
  int y = lv_obj_get_y(dot_player);
  if (gpio == INPUT_DOWN_BTN) {
    lv_obj_set_pos(dot_player, x, y + DY);
  } else {
    lv_obj_set_pos(dot_player, x, y - DY);
  }
}
