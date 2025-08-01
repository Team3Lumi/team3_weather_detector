#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Gán chân nút
#define GPIO_BACK    16
#define GPIO_NEXT    15
#define GPIO_OK      7  // chưa dùng
#define GPIO_CANCEL  7

#define TAG "WEATHER_UI"

// Số ngày tối đa (0 -> 6)
#define MAX_STATE 6

// Biến trạng thái hiện tại
int current_state = 0;

// Hàm hiển thị trạng thái ra Serial (giả lập màn hình TFT)
void update_display() {
    ESP_LOGI(TAG, "=== Dự báo thời tiết: Ngày D+%d ===", current_state);
    // TODO: Hiển thị màn hình TFT tương ứng nếu cần
}

// Cấu hình GPIO cho nút nhấn
void button_init() {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << GPIO_BACK) | (1ULL << GPIO_NEXT) | (1ULL << GPIO_CANCEL),
        .pull_down_en = 0,
        .pull_up_en = 1
    };
    gpio_config(&io_conf);
}

// Nhiệm vụ đọc nút và xử lý máy trạng thái
void state_machine_task(void *pvParameter) {
    int prev_back = 1, prev_next = 1, prev_cancel = 1;

    while (1) {
        int back = gpio_get_level(GPIO_BACK);
        int next = gpio_get_level(GPIO_NEXT);
        int cancel = gpio_get_level(GPIO_CANCEL);

        // Phát hiện cạnh xuống (nhấn nút BACK)
        if (back == 0 && prev_back == 1) {
            if (current_state > 0) {
                current_state--;
                update_display();
            }
        }
        prev_back = back;

        // Phát hiện cạnh xuống (nhấn nút NEXT)
        if (next == 0 && prev_next == 1) {
            if (current_state < MAX_STATE) {
                current_state++;
                update_display();
            }
        }
        prev_next = next;

        // Phát hiện cạnh xuống (nhấn nút CANCEL → về ngày hôm nay)
        if (cancel == 0 && prev_cancel == 1) {
            if (current_state != 0) {
                current_state = 0;
                update_display();
            }
        }
        prev_cancel = cancel;

        vTaskDelay(pdMS_TO_TICKS(100)); // debounce 100ms
    }
}

void app_main() {
    button_init();
    update_display(); // Mặc định hiện ngày hôm nay
    xTaskCreate(state_machine_task, "state_machine_task", 2048, NULL, 5, NULL);
}
