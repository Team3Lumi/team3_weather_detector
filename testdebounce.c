#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_GPIO GPIO_NUM_6
volatile bool button_pressed = false;

static void IRAM_ATTR button_isr_handler(void* arg) {
    button_pressed = true;  // Đặt cờ khi nút được nhấn
}

void app_main(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE  // Ngắt cạnh xuống (nhấn)
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0);  // Bắt đầu dịch vụ ISR
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

    while (1) {
        if (button_pressed) {
            button_pressed = false;
            printf("Button Pressed (interrupt)\n");

            // Chống dội đơn giản
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        vTaskDelay(pdMS_TO_TICKS(10));  // Chu kỳ kiểm tra
    }
}
