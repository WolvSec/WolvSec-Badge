#ifndef LCD_HAL_H
#define LCD_HAL_H

#include <stddef.h>
#include <stdint.h>

void lcd_hal_init(void);
void lcd_hal_reset(void);
void lcd_hal_send_cmd(const uint8_t *cmd, size_t cmd_size);
void lcd_hal_send_data(const uint8_t *data, size_t data_size);
void lcd_hal_send_data_dma(const uint8_t *data, size_t data_size,
                           void (*cb)(void *), void *cb_data);

#endif