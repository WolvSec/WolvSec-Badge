#ifndef LCD_IMPL_H
#define LCD_IMPL_H

#include "lvgl.h"
#include "lv_conf.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define LCD_RES_X 240
#define LCD_RES_Y LCD_RES_X

/**
 * GPIO
 */
#define LCD_PIN_CK 2
#define LCD_PIN_SI 3
#define LCD_PIN_RST 20
#define LCD_PIN_TC 5
#define LCD_PIN_DC 6

#define SPI_PORT spi0
#define LCD_SPI_SPEED 1000000 // ?

/* Initialize communication with the LCD. */
void lcd_spi_init();

/* Set our LCD to simplify IRQ callbacks. */
void lcd_set_ctx(lv_display_t *lcd);

/* Send short command to the LCD. This function shall wait until the transaction finishes. */
int32_t my_lcd_send_cmd(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size);

/* Send large array of pixel data to the LCD. Use DMA. */
int32_t my_lcd_send_color_dma(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size);

/* Send large array of pixel data to the LCD. Should only be used in cases where DMA is not an option. */
int32_t my_lcd_send_color(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size);

#endif
