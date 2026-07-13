#include <freertos\FreeRTOS.h>
#include <freertos\task.h>
#include <freertos\queue.h>
#include <esp_system.h>
#include <sdkconfig.h>
#include <driver/uart.h>
#include <soc/gpio_reg.h> 
#include <driver/gpio.h>
#include <soc/uart_reg.h>
#include "esp_partition.h"
#include "esp_ota_ops.h"

#define BAUD 115200
#define QUEUE_SIZE 20
#define RX_FLOW_CTRL_THRESH 120
#define SECOND 1000

extern void blink_function(uint16_t count, uint16_t delay);
extern void delay(uint16_t count);
extern uint8_t button_function(void);

void init_uart(void);
void init_gpio(void);
void message_about_current_work_once_per_second_uart(const char* message, uint16_t uart);
void jump_to_program3(void);

// void init_uart(void)
// {
//     const int uart_buffer_size = (1024 * 2);
//     QueueHandle_t uart_queue;

//     // установка драйвера UART через функцию проверки ошибки 
//     ESP_ERROR_CHECK( uart_driver_install(UART_NUM_1, uart_buffer_size, uart_buffer_size, QUEUE_SIZE, &uart_queue, 0) ); 
//     printf("Установлен драйвер UART: %i\n", uart_is_driver_installed(UART_NUM_1));

//     // инициализация структуры конфигурации UART и установка параметров UART
//     uart_config_t uart_conf = {0}; 
//     uart_conf.baud_rate = BAUD;
//     uart_conf.data_bits = UART_DATA_8_BITS;
//     uart_conf.parity = UART_PARITY_DISABLE;
//     uart_conf.stop_bits = UART_STOP_BITS_1;
//     uart_conf.flow_ctrl = UART_HW_FLOWCTRL_RTS;
//     uart_conf.rx_flow_ctrl_thresh = RX_FLOW_CTRL_THRESH;
//     uart_conf.source_clk = UART_SCLK_DEFAULT;

//     uart_intr_config_t uart_intr_conf = {0};
//     uart_intr_conf.intr_enable_mask = UART_RXFIFO_FULL_INT_ENA_M | UART_RXFIFO_TOUT_INT_ENA_M;
//     uart_intr_conf.rx_timeout_thresh = 10;
//     uart_intr_conf.txfifo_empty_intr_thresh = 10;
//     uart_intr_conf.rxfifo_full_thresh = 120;

//     // установка пинов UART через функцию проверки ошибки
//     ESP_ERROR_CHECK( uart_set_pin(UART_NUM_1, GPIO_NUM_2, GPIO_NUM_8, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) ); 
//     ESP_ERROR_CHECK( uart_param_config(UART_NUM_1, &uart_conf) );     // установка параметров UART
//     ESP_ERROR_CHECK( uart_intr_config(UART_NUM_1, &uart_intr_conf) ); // установка прерывания UART
// }

// void init_gpio(void)
// {
//     gpio_config_t gpio_8_conf = {0}; // инициализация структуры конфигурации GPIO 8 - светодиод 
//     gpio_8_conf.pin_bit_mask = (1ULL << GPIO_NUM_8);
//     gpio_8_conf.mode = GPIO_MODE_OUTPUT;
//     gpio_8_conf.pull_up_en = GPIO_PULLUP_DISABLE;
//     gpio_8_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
//     gpio_8_conf.intr_type = GPIO_INTR_DISABLE;

//     gpio_config_t gpio_2_conf = {0}; // инициализация структуры конфигурации GPIO 2 - кнопка
//     gpio_2_conf.pin_bit_mask = (1ULL << GPIO_NUM_2);
//     gpio_2_conf.mode = GPIO_MODE_INPUT;
//     gpio_2_conf.pull_up_en = GPIO_PULLUP_ENABLE;
//     gpio_2_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
//     gpio_2_conf.intr_type = GPIO_INTR_DISABLE;

//     ESP_ERROR_CHECK( gpio_config(&gpio_8_conf) );
//     ESP_ERROR_CHECK( gpio_config(&gpio_2_conf) );
// }

void message_about_current_work_once_per_second_uart(const char* message, uint16_t uart)
{
    uart_write_bytes(uart, message, strlen(message));
    uart_flush(uart);
    delay(SECOND);
}

// void jump_to_program3(void)
// {
//     const esp_partition_t* program3_partition = NULL;
//     esp_err_t find_partition_error = esp_partition_find_first_err
//     (
//         ESP_PARTITION_TYPE_ANY,
//         ESP_PARTITION_SUBTYPE_ANY,
//         "program3",
//         &program3_partition
//     );

//     if(find_partition_error != ESP_OK)
//     {
//         const char error_message[] = "Ошибка поиска раздела program2: ";
//         uart_write_bytes(UART_NUM_1, error_message, strlen(error_message));
//         uart_write_bytes(UART_NUM_1, esp_err_to_name(find_partition_error), strlen(esp_err_to_name(find_partition_error)));
//         uart_flush(UART_NUM_1); // очищаем буфер
//         delay(200);
//         return;
//     }

//     ESP_ERROR_CHECK( esp_ota_set_boot_partition(program3_partition) ); // установка раздела program3 как загрузочного раздела для следующей перезагрузки
//     const char msg[] = "Переход к Program3...\n";
//     uart_write_bytes(UART_NUM_1, msg, strlen(msg));
//     uart_flush(UART_NUM_1); // очищаем буфер
//     delay(200);
//     esp_restart(); // перезагрузка микроконтроллера для перехода в другую программу
// }

void app_main() 
{
    // init_uart();
    // init_gpio();
    blink_function(2, 500);
    const char str[] = "Program2: Работаю...";
    while(1)
    {
        message_about_current_work_once_per_second_uart(str, UART_NUM_1);
        if(button_function()){ break; }
    }
    delay(500);
    //jump_to_program3();
}