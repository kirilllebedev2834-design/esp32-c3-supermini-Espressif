//#include <freertos\FreeRTOS.h>
//#include <freertos\task.h>
#include <esp_system.h>
#include <sdkconfig.h>
//#include <locale.h>
#include <stdio.h>
#include <driver/uart.h>
#include <soc/gpio_reg.h>
#include <driver/gpio.h>

extern void blink_function(uint16_t count, uint16_t delay);
extern void delay(uint16_t count);

// функция инициализации UART
void unit_uart()
{

}

// функция инициализации GPIO
// void unit_gpio()
// {
//     gpio_config_t config = 
//     {
//         .pin_bit_mask = (1ULL << GPIO_NUM_8),
//         .mode = GPIO_MODE_OUTPUT
//     };
//     gpio_config(&config);
// }
    
void app_main() 
{
    //setlocale(LC_ALL, "Russian");
    //esp_err_t a;
    gpio_dump_io_configuration(stdout, SOC_GPIO_VALID_GPIO_MASK);
    
    //blink_function(4, 500);
    
    //GPIO_OUT_W1TS_REG; // адрес регистра HIGH - 1 0x60004000 + 0x8
    //GPIO_OUT_W1TC_REG; // адрес регистра LOW - 0  0x60004000 + 0xC


}