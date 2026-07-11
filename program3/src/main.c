#include <freertos\task.h>
#include <esp_system.h>
#include <sdkconfig.h>
#include <driver/uart.h>
#include <soc/gpio_reg.h> // - макросы
#include <driver/gpio.h>

extern void blink_function(uint16_t count, uint16_t delay);
extern void delay(uint16_t count);

// функция инициализации UART
void unit_uart()
{
    uart_config_t uart_conf = {}; // инициализация структуры конфигурации UART
    uart_conf.baud_rate = 115200;
    
    uart_write_bytes(); // запись сообщения через UART
    uart_flush();       // стираем запись
}

// функция инициализации GPIO 
void unit_gpio()
{
    gpio_config_t gpio_conf = {}; // инициализация структуры конфигурации GPIO 
    
    gpio_conf.pin_bit_mask = (1ULL << GPIO_NUM_8);
    gpio_conf.mode = GPIO_MODE_OUTPUT;

    gpio_config(&gpio_conf);
}

void app_main() 
{

}