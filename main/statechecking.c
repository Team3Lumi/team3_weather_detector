#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Định nghĩa chân nút nhấn
#define BTN_BACK     GPIO_NUM_16
#define BTN_NEXT     GPIO_NUM_15
#define BTN_OK       GPIO_NUM_6
#define BTN_CANCEL   GPIO_NUM_7

static const char *TAG = "BUTTON_CHECK";

// Hàm khởi tạo GPIO cho nút nhấn
void button_init() {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,        // Không dùng ngắt
        .mode = GPIO_MODE_INPUT,               // Dùng làm đầu vào
        .pin_bit_mask = (1ULL << BTN_BACK) |
                        (1ULL << BTN_NEXT) |
                        (1ULL << BTN_OK) |
                        (1ULL << BTN_CANCEL),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE       // Kích hoạt trở kéo lên nội bộ
    };
    gpio_config(&io_conf);
}

// Hàm đọc trạng thái nút nhấn (1: không nhấn, 0: nhấn)
int read_button(gpio_num_t pin) {
    return gpio_get_level(pin);
}

// Task kiểm tra nút nhấn và in ra serial
void button_task(void *pvParameters) {
    int back_prev = 1, next_prev = 1, ok_prev = 1, cancel_prev = 1;

    while (1) {
        int back = read_button(BTN_BACK);
        int next = read_button(BTN_NEXT);
        int ok = read_button(BTN_OK);
        int cancel = read_button(BTN_CANCEL);

        // Kiểm tra từng nút với trạng thái trước đó (chống dội cơ bản)
        if (back == 0 && back_prev == 1) {
            ESP_LOGI(TAG, "Nút BACK được nhấn");
        }
        if (next == 0 && next_prev == 1) {
            ESP_LOGI(TAG, "Nút NEXT được nhấn");
        }
        if (ok == 0 && ok_prev == 1) {
            ESP_LOGI(TAG, "Nút OK được nhấn");
        }
        if (cancel == 0 && cancel_prev == 1) {
            ESP_LOGI(TAG, "Nút CANCEL được nhấn");
        }

        // Cập nhật trạng thái trước đó
        back_prev = back;
        next_prev = next;
        ok_prev = ok;
        cancel_prev = cancel;

        vTaskDelay(pdMS_TO_TICKS(50)); // Delay 50ms để chống dội
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Khởi động chương trình kiểm tra nút nhấn...");

    button_init(); // Cấu hình các nút
    xTaskCreate(button_task, "button_task", 2048, NULL, 5, NULL);
}
