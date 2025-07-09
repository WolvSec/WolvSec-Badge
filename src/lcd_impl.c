#include "lcd_impl.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>

#define LCD_DMA_ENABLED 1

void lcd_init() {
    printf("lcd spi init\n");
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
}

void lcd_dma_init(lv_display_t *lcd) {
    lcd_user_data *user_data;
    user_data = lv_calloc(1, sizeof(lcd_user_data));
    if (!user_data) {
        printf("Could not allocate user data for lcd display. Out of memory.\n");
    }

    user_data->dma_chan = dma_claim_unused_channel(true);
    user_data->dma_conf = dma_channel_get_default_config(user_data->dma_chan);
    channel_config_set_transfer_data_size(&user_data->dma_conf, DMA_SIZE_8);
    channel_config_set_dreq(&user_data->dma_conf, spi_get_dreq(SPI_PORT, true));

    lv_display_set_user_data(lcd, user_data);
}

static void lcd_dma_transfer_complete(lv_display_t *disp) {
    lv_display_flush_ready(disp);
}

static void lcd_spi_write(const uint8_t *data, size_t length) {
    gpio_put(LCD_PIN_TC, 0);
    spi_write_blocking(SPI_PORT, data, length);
    gpio_put(LCD_PIN_TC, 1);
}

int32_t lcd_send_cmd(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size)
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

int32_t lcd_send_color_dma(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size)
{
    lcd_user_data *user_data;

    printf("sending color cmd: 0x%02x\n", cmd);
    gpio_put(LCD_PIN_DC, 0);
    lcd_spi_write(cmd, cmd_size);

    if (!param || param_size < 1) {
        return 0;
    }

    if (LCD_DMA_ENABLED) {
        user_data = (lcd_user_data*) lv_display_get_user_data(disp);
        if (!user_data)
            return 0;

        /* Use DMA for writing large pixel array. */
        dma_channel_configure(user_data->dma_chan, &user_data->dma_conf,
            &spi_get_hw(SPI_PORT)->dr,
            param,
            param_size,
            false);

        if (dma_channel_is_busy(user_data->dma_chan)) {
            printf("ERROR: DMA channel was busy. LVGL should have handled this.\n");
            return 0;
        }

        /* Begin a DMA transfer. */
        gpio_put(LCD_PIN_TC, 0);
        gpio_put(LCD_PIN_DC, 1);
        dma_channel_start(user_data->dma_chan);

        /* TODO: Replace with non-blocking IRQ handler. */
        dma_channel_wait_for_finish_blocking(user_data->dma_chan);
        lcd_dma_transfer_complete(disp);
        return 0;
    }
    if (param && param_size > 0) {
        gpio_put(LCD_PIN_DC, 1);
        lcd_spi_write(param, param_size);
    }
    lv_display_flush_ready(disp);
    return 0;
}

int32_t lcd_send_color(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size)
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
