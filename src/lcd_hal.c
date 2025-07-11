#include "lcd_hal.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <stdio.h>

/**
 * GPIO
 */
#define LCD_PIN_CK 2
#define LCD_PIN_SI 3
#define LCD_PIN_RST 20
#define LCD_PIN_TC 5
#define LCD_PIN_DC 6

#define SPI_PORT spi0
#define LCD_SPI_SPEED 1000000

static int dma_chan;
static dma_channel_config dma_cfg;

void lcd_hal_init(void) {
  spi_init(SPI_PORT, LCD_SPI_SPEED);
  gpio_set_function(LCD_PIN_CK, GPIO_FUNC_SPI);
  gpio_set_function(LCD_PIN_SI, GPIO_FUNC_SPI);

  gpio_init(LCD_PIN_TC);
  gpio_set_dir(LCD_PIN_TC, GPIO_OUT);
  gpio_put(LCD_PIN_TC, 1);

  gpio_init(LCD_PIN_DC);
  gpio_set_dir(LCD_PIN_DC, GPIO_OUT);

  gpio_init(LCD_PIN_RST);
  gpio_set_dir(LCD_PIN_RST, GPIO_OUT);

  lcd_hal_reset();

  dma_chan = dma_claim_unused_channel(true);
  dma_cfg = dma_channel_get_default_config(dma_chan);
  channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_8);
  channel_config_set_dreq(&dma_cfg, spi_get_dreq(SPI_PORT, true));
}

void lcd_hal_reset(void) {
  gpio_put(LCD_PIN_RST, 0);
  sleep_ms(10);
  gpio_put(LCD_PIN_RST, 1);
  sleep_ms(10);
}

void lcd_hal_send_cmd(const uint8_t *cmd, size_t cmd_size) {
  gpio_put(LCD_PIN_DC, 0);
  gpio_put(LCD_PIN_TC, 0);
  spi_write_blocking(SPI_PORT, cmd, cmd_size);
  gpio_put(LCD_PIN_TC, 1);
}

void lcd_hal_send_data(const uint8_t *data, size_t data_size) {
  gpio_put(LCD_PIN_DC, 1);
  gpio_put(LCD_PIN_TC, 0);
  spi_write_blocking(SPI_PORT, data, data_size);
  gpio_put(LCD_PIN_TC, 1);
}

void lcd_hal_send_data_dma(const uint8_t *data, size_t data_size,
                           void (*cb)(void *), void *cb_data) {
  gpio_put(LCD_PIN_DC, 1);
  gpio_put(LCD_PIN_TC, 0);

  dma_channel_configure(dma_chan, &dma_cfg, &spi_get_hw(SPI_PORT)->dr, data,
                        data_size, false);
  dma_channel_start(dma_chan);

  dma_channel_wait_for_finish_blocking(dma_chan);

  gpio_put(LCD_PIN_TC, 1);

  if (cb)
    cb(cb_data);
}