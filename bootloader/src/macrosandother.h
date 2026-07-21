#ifndef MACROSES_AND_OTHERS_FOR_ESP32
#define MACROSES_AND_OTHERS_FOR_ESP32

/*
   GPIO     - 0x6000_4000 - 0x6000_4FFF 176 страница
   IO_MUX   - 0x6000_9000 - 0x6000_9FFF после GPIO
   UART     - 0x6000_0000 - 0x6000_0FFF 561 и 568 страницы 
   203 211 276 294 432
*/

#include <stdint.h>

#define true ((_Bool)1)
#define false ((_Bool)0)
#define bool _Bool
// UART macroses for work from documentation:
#define UART1_BASE_ADRESS 0x60010000
#define UART1_CLK_DEV_REG 0x60010014

//GPIO and IO_MUX macroses for work from documentation: 
#define GPIO_OUTPUT_ENABLE_REGISTER 0x60004020
#define GPIO_OUTPUT_ENABLE_SET_REGISTER 0x60004024
#define GPIO_OUTPUT_ENABLE_CLEAR_REGISTER 0x60004028
#define IO_MUX_CONF_REG_FOR_PIN_GPIO9 0x60009028
#define IO_MUX_CONF_REG_FOR_PIN_GPIO8 0x60009024
#define IO_MUX_CONF_REG_FOR_PIN_GPIO5 0x6000901C
#define IO_MUX_CONF_REG_FOR_PIN_GPIO4 0x60009018


typedef uint8_t byte;

extern void delay(uint16_t milliseconds); 
extern void blink_function(uint16_t count, uint32_t delay_milliseconds);
void init_uart1(void);
void init_gpio(void);

#endif