#include "lcd_impl.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>

int dma_tx = 0;
dma_channel_config dma_tx_conf;

/* Save our LCD display for use in IRQ callbacks */
lv_display_t *lcd_ctx;

static void lcd_spi_write(const uint8_t *data, size_t length) {
    gpio_put(LCD_PIN_TC, 0);
    printf("writing %ld bytes to spi interface\n", length);
    spi_write_blocking(SPI_PORT, data, length);
    gpio_put(LCD_PIN_TC, 1);
}

static void dma_transfer_complete(lv_display_t *disp) {
    lv_display_flush_ready(disp);
}

static void lcd_dma_irq_cb() {
    /* Handle the IRQ */
    gpio_put(LCD_PIN_TC, 1);
    if (!lcd_ctx) {
        printf("Error: lcd_ctx was not set. DMA transfer will not be completed.\n");
        return;
    }
    dma_transfer_complete(lcd_ctx);
    dma_channel_acknowledge_irq0(dma_tx);
    printf("DMA IRQ fired\n");
}

void lcd_spi_init() {
    spi_init(SPI_PORT, LCD_SPI_SPEED);
    gpio_set_function(LCD_PIN_CK, GPIO_FUNC_SPI);
    gpio_set_function(LCD_PIN_SI, GPIO_FUNC_SPI);

    // CS
    gpio_init(LCD_PIN_TC);
    gpio_set_dir(LCD_PIN_TC, GPIO_OUT);
    gpio_put(LCD_PIN_TC, 1);

    // DC
    gpio_init(LCD_PIN_DC);
    gpio_set_dir(LCD_PIN_DC, GPIO_OUT);

    // RST
    gpio_init(LCD_PIN_RST);
    gpio_set_dir(LCD_PIN_RST, GPIO_OUT);
    gpio_put(LCD_PIN_RST, 0);
    gpio_put(LCD_PIN_RST, 1);

    /**
    * DMA channel configuration for SPI interface.
    */
    dma_tx = dma_claim_unused_channel(true);
    dma_tx_conf = dma_channel_get_default_config(dma_tx);
    channel_config_set_transfer_data_size(&dma_tx_conf, DMA_SIZE_8);
    channel_config_set_dreq(&dma_tx_conf, spi_get_dreq(SPI_PORT, true));
    // dma_channel_set_irq0_enabled(dma_tx, true);
    // irq_set_exclusive_handler(DMA_IRQ_0, lcd_dma_irq_cb);
    // irq_set_enabled(DMA_IRQ_0, true);
}

void lcd_set_ctx(lv_display_t *lcd) {
    lcd_ctx = lcd;
}

int32_t my_lcd_send_cmd(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size)
{
    printf("cmd: %x\ncmd_size: %ld\nparam: %x\nparam_size: %ld\n", *cmd, cmd_size, *param, param_size);
    gpio_put(LCD_PIN_DC, 0);
    lcd_spi_write(cmd, cmd_size);
    if (param && param_size > 0) {
        gpio_put(LCD_PIN_DC, 1);
        lcd_spi_write(param, param_size);
    }
    lv_display_flush_ready(disp);
    return 0;
}

int32_t my_lcd_send_color_dma(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size)
{
    printf("sending color cmd: 0x%02x\n", cmd);
    gpio_put(LCD_PIN_DC, 0);
    lcd_spi_write(cmd, cmd_size);

    if (!param || param_size < 1) {
        return 0;
    }

    /* Use DMA for writing large pixel array. */
    dma_channel_configure(dma_tx, &dma_tx_conf,
        &spi_get_hw(SPI_PORT)->dr,
        param,
        param_size,
        false);

    if (dma_channel_is_busy(dma_tx)) {
        printf("ERROR: DMA channel was busy. LVGL should have handled this.");
        return 0;
    }

    /* Begin a DMA transfer. */
    gpio_put(LCD_PIN_TC, 0);
    gpio_put(LCD_PIN_DC, 1);
    dma_channel_start(dma_tx);

    /* TODO: Replace with non-blocking IRQ handler. */
    dma_channel_wait_for_finish_blocking(dma_tx);
    dma_transfer_complete(disp);
    return 0;
}

int32_t my_lcd_send_color(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size)
{
    printf("sending color cmd: 0x%02x\n", cmd);
    gpio_put(LCD_PIN_DC, 0);
    lcd_spi_write(cmd, cmd_size);
    if (param && param_size > 0) {
        gpio_put(LCD_PIN_DC, 1);
        lcd_spi_write(param, param_size);
    }
    lv_display_flush_ready(disp);
    return 0;
}
