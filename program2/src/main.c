#include <freertos\task.h>
#include <esp_system.h>
#include <sdkconfig.h>
#include <driver/uart.h>
#include <soc/gpio_reg.h> // - макросы
#include <driver/gpio.h>

extern void blink_function(uint16_t count, uint16_t delay);
extern void delay(uint16_t count);


void app_main() 
{

}