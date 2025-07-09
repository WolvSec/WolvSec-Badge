/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "lcd_impl.h"
#include "logo.h"
#include <stdio.h> /* For debug */

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 1000
#endif

lv_display_t *lcd;
repeating_timer_t lvgl_timer;
lv_color_t * buf1 = NULL;
lv_color_t * buf2 = NULL;
uint32_t buf_size = 0;

static bool repeating_lvgl_timer_callback(struct repeating_timer *t)
{
    lv_tick_inc(5);
    return true;
}

void ui_logo_init(void)
{
    LV_IMAGE_DECLARE(cat_img);
    lv_obj_t * img1 = lv_image_create(lv_screen_active());
    lv_image_set_src(img1, &cat_img);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
}

void ui_init(lv_display_t *disp)
{
    lv_obj_t *obj;

    /* set screen background to white */
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_100, 0);

    /* create label */
    obj = lv_label_create(scr);
    lv_obj_set_align(obj, LV_ALIGN_CENTER);
    lv_obj_set_height(obj, LV_SIZE_CONTENT);
    lv_obj_set_width(obj, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(obj, lv_color_make(0,255,0), 0);
    lv_label_set_text(obj, "Hello World!");
}

int pico_display_init(void) {
    lv_init();
    lcd_init();
    add_repeating_timer_ms(5, repeating_lvgl_timer_callback, NULL, &lvgl_timer);
    lcd = lv_st7789_create(LCD_RES_X, LCD_RES_Y, 0, (lv_st7789_send_cmd_cb_t) lcd_send_cmd, (lv_st7789_send_color_cb_t) lcd_send_color_dma);
    lcd_dma_init(lcd);
    if (!lcd) {
        printf("lv_st7789_create failed with code: %d\n", lcd);
        return -1;
    }
    //lcd_init(lcd);
    buf_size = LCD_RES_X * LCD_RES_Y / 10 * lv_color_format_get_size(lv_display_get_color_format(lcd));
    buf1 = lv_malloc(buf_size);
    if(buf1 == NULL) {
            printf("display draw buffer malloc failed\n");
            return -1;
    }

    buf2 = lv_malloc(buf_size);
    if(buf2 == NULL) {
            printf("display buffer malloc failed\n");
            lv_free(buf1);
            return -1;
    }
    lv_display_set_buffers(lcd, buf1, buf2, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_st7789_set_invert(lcd, 1);
    ui_logo_init();
    return PICO_OK;
}

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
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

int main() {
    stdio_init_all();
    sleep_ms(5000);
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    int check = pico_display_init();
    while (true) {
        lv_timer_handler();
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
    }
}
