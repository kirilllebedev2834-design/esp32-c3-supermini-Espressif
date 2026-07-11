#include <freertos\FreeRTOS.h>
#include <freertos\task.h>
#include <freertos\queue.h>
#include <esp_system.h>
#include <sdkconfig.h>
#include <driver/uart.h>
#include <soc/gpio_reg.h> 
#include <driver/gpio.h>
#include <soc/uart_reg.h>
//#include <esp_intr_alloc.h>

#define QUEUE_SIZE 10
#define BAUD 115200
#define RX_FLOW_CTRL_THRESH 120

extern void delay(uint16_t count);
void init_uart();
void init_gpio();
void jump_to_program();

// gpio_hold_en(GPIO_NUM_8);
// gpio_hold_dis(GPIO_NUM_8);

/* 
   Функция инициализации UART
   const int uart_buffer_size - размер буфера RX и TX UART, который будет использоваться для приема и передачи данных.
   QueueHandle_t uart_queue - очередь для хранения событий UART, таких как получение данных или ошибки.
   uart_driver_install() - установка драйвера UART с указанными параметрами. 
   инициализация структуры конфигурации UART и установка параметров UART через uart_param_config().
   устанавливает пины UART через функцию uart_set_pin().
   устанавливает параметры UART через функцию uart_param_config().
   Сама функция ничего не возвращает, но ESP_ERROR_CHECK будет возвращать состояния.
*/ 
void init_uart()
{
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;

    // установка драйвера UART через функцию проверки ошибки 
    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_1, uart_buffer_size, uart_buffer_size, QUEUE_SIZE, &uart_queue, 0) ); 
    printf("Установлен драйвер UART: %i\n", uart_is_driver_installed(UART_NUM_1));

    // инициализация структуры конфигурации UART и установка параметров UART
    uart_config_t uart_conf = {0}; 
    uart_conf.baud_rate = BAUD;
    uart_conf.data_bits = UART_DATA_8_BITS;
    uart_conf.parity = UART_PARITY_DISABLE;
    uart_conf.stop_bits = UART_STOP_BITS_1;
    uart_conf.flow_ctrl = UART_HW_FLOWCTRL_RTS;
    uart_conf.rx_flow_ctrl_thresh = RX_FLOW_CTRL_THRESH;
    uart_conf.source_clk = UART_SCLK_DEFAULT;

    uart_intr_config_t uart_intr_conf = {0};
    uart_intr_conf.intr_enable_mask = UART_RXFIFO_FULL_INT_ENA_M | UART_RXFIFO_TOUT_INT_ENA_M;
    uart_intr_conf.rx_timeout_thresh = 10;
    uart_intr_conf.txfifo_empty_intr_thresh = 10;
    uart_intr_conf.rxfifo_full_thresh = 120;

    // установка пинов UART через функцию проверки ошибки
    ESP_ERROR_CHECK( uart_set_pin(UART_NUM_1, 2, 8, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) ); 
    ESP_ERROR_CHECK( uart_param_config(UART_NUM_1, &uart_conf) );     // установка параметров UART
    ESP_ERROR_CHECK( uart_intr_config(UART_NUM_1, &uart_intr_conf) ); // установка прерывания UART
}

/* 
   Функция инициализации GPIO 
   Иниицализация двух структур конфигурации GPIO: GPIO 8 - синий светодиод на микроконтроллере ESP32-C3
   и GPIO 2 - пин на плате ESP32-C3.
   GPIO 8 настраивается как выходной пин, а GPIO 2 - как входной пин с подтяжкой к питанию и прерыванием по любому фронту.
   Настраиваем общую конфигурацию через gpio_config() и устанавливаем службу обработчика прерываний через gpio_install_isr_service().
   Сама функция ничего не возвращает, но ESP_ERROR_CHECK будет возвращать состояния.
*/ 
void init_gpio()
{
    gpio_config_t gpio_8_conf = {0}; // инициализация структуры конфигурации GPIO 8 - светодиод 
    gpio_8_conf.pin_bit_mask = (1ULL << GPIO_NUM_8);
    gpio_8_conf.mode = GPIO_MODE_OUTPUT;
    gpio_8_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_8_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_8_conf.intr_type = GPIO_INTR_DISABLE;

    gpio_config_t gpio_2_conf = {0}; // инициализация структуры конфигурации GPIO 2 - кнопка
    gpio_2_conf.pin_bit_mask = (1ULL << GPIO_NUM_2);
    gpio_2_conf.mode = GPIO_MODE_INPUT;
    gpio_2_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_2_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_2_conf.intr_type = GPIO_INTR_ANYEDGE;

    ESP_ERROR_CHECK( gpio_config(&gpio_8_conf) );
    ESP_ERROR_CHECK( gpio_config(&gpio_2_conf) );
    
    /*
    функция службы обработчика прерываний драйвера GPIO для 
    создания обработчиков прерываний каждого контакта GPIO
    */ 
    ESP_ERROR_CHECK( gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1) );
}

/*
    Функция прыжка в другую программу
*/
void jump_to_program()
{

}

void app_main() 
{
    const char message[] = "Инициализация завершена. Переход к Program2.\n";
    //gpio_dump_io_configuration(stdout, SOC_GPIO_VALID_GPIO_MASK);
    init_uart(); 
    init_gpio(); 
    uart_write_bytes(UART_NUM_1, message, strlen(message)); // запись сообщения через UART
    delay(1000);
    uart_flush(UART_NUM_1); // очищаем буфер 
    //jump_to_program();
}