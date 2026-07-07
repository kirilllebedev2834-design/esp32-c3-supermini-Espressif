#include <freertos\FreeRTOS.h>
#include <freertos\task.h>
#include <freertos\queue.h>
#include <esp_system.h>
#include <sdkconfig.h>
#include <driver/uart.h>
#include <soc/gpio_reg.h> // - макросы
#include <driver/gpio.h>
#include <soc/uart_reg.h>
//#include <string.h>

#define QUEUE_SIZE 10
#define BAUD 115200

extern void delay(uint16_t count);

/*функция инициализации UART


*/ 
void unit_uart()
{
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;

    // установка драйвера UART через функцию проверки ошибки 
    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_1, uart_buffer_size, uart_buffer_size, QUEUE_SIZE, &uart_queue, 0) ); 

    // инициализация структуры конфигурации UART и установка параметров UART
    uart_config_t uart_conf = {0}; 
    uart_conf.baud_rate = BAUD;
    uart_conf.data_bits = UART_DATA_8_BITS;
    uart_conf.parity = UART_PARITY_DISABLE;
    uart_conf.stop_bits = UART_STOP_BITS_1;
    uart_conf.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_conf.rx_flow_ctrl_thresh = 122;

    // установка пинов UART через функцию проверки ошибки
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, GPIO_NUM_2, GPIO_NUM_4, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)); 
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_conf));  // установка параметров UART через функцию проверки ошибки

}

// функция инициализации GPIO 
void unit_gpio()
{

    gpio_config_t gpio_conf = {0}; // инициализация структуры конфигурации GPIO 
    gpio_conf.pin_bit_mask = (1ULL << GPIO_NUM_8);
    gpio_conf.mode = GPIO_MODE_OUTPUT;
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&gpio_conf));
}
    
void app_main() 
{
    const char* message = "Инициализация завершена. Переход к Program2.\n";
    //gpio_dump_io_configuration(stdout, SOC_GPIO_VALID_GPIO_MASK);
    unit_uart(); // инициализация UART
    unit_gpio(); // инициализация GPIO
    uart_write_bytes(UART_NUM_1, message, strlen(message)); // запись сообщения через UART
    delay(1000);
}