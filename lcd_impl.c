#include "lcd_impl.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>

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
}

void lcd_spi_write(const uint8_t *data, size_t length) {
    gpio_put(LCD_PIN_TC, 0);
    printf("writing %ld bytes to spi interface\n", length);
    spi_write_blocking(SPI_PORT, data, length);
    gpio_put(LCD_PIN_TC, 1);
}

/* Send short command to the LCD. This function shall wait until the transaction finishes. */
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

/* Send large array of pixel data to the LCD. If necessary, this function has to do the byte-swapping. This function can do the transfer in the background. */
int32_t my_lcd_send_color(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size)
{
    /**
     * TODO: Implement non-blocking: https://docs.lvgl.io/master/details/integration/driver/display/gen_mipi.html
     */
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
