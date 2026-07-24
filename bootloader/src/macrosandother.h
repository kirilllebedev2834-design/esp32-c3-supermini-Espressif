#ifndef MACROSES_AND_OTHERS_FOR_ESP32
#define MACROSES_AND_OTHERS_FOR_ESP32

/*
   GPIO     - 0x6000_4000 - 0x6000_4FFF 176 страница
   IO_MUX   - 0x6000_9000 - 0x6000_9FFF после GPIO
   UART     - 0x6000_0000 - 0x6000_0FFF 561 и 568 страницы 
   203 211 276 294 432
*/

#include <stdint.h>
#include <stdio.h>

#define true ((_Bool)1)
#define false ((_Bool)0)
#define bool _Bool
#define REGISTER_POINTER(adress) (*(volatile uint32_t*)(adress))

// UART macroses for work from documentation:
#define UART_FIFO_REG 0x60000000  // Буфер приема/передачи (FIFO)
#define UART_CLKDIV_REG 0x60000014  // Делитель частоты (скорость)
#define UART_STATUS_REGISTER 0x6000001C
#define UART_CONF_REG  0x60000020  // Конфигурация кадра
#define UART_CLK_CONF_REG 0x60000078  // Настройка частоты ядра
#define UART_UPDATE_REG 0x600000A4  // Синхронизация регистров
#define SYS_CLK_EN_REG 0x600C0010  // Включение тактирования периферии
#define SYS_RST_EN_REG 0x600C0018  // Сброс периферии

//GPIO and IO_MUX macroses for work from documentation:
#define GPIO_OUTPUT_SET_REGISTER 0x60004008 
#define GPIO_OUTPUT_CLEAR_REGISTER 0x6000400C
#define GPIO_OUTPUT_ENABLE_REGISTER 0x60004020
#define GPIO_OUTPUT_ENABLE_SET_REGISTER 0x60004024
#define GPIO_OUTPUT_ENABLE_CLEAR_REGISTER 0x60004028
#define IO_MUX_CONF_REG_FOR_PIN_GPIO9 0x60009028
#define IO_MUX_CONF_REG_FOR_PIN_GPIO8 0x60009024

typedef uint8_t byte;

// перечесления ошибок 
typedef enum
{
   SYSTEM_IS_OK = 0,
   SYSTEM_TIMEOUT = 1,
   SYSTEM_INVALID_ARG = 2,
   SYSTEM_PER_IS_NOT_READY = 3,
   SYSTEM_INVALID_JUMP_TO_PROGRAM = 4,
} error_status_t; 


extern void delay(uint16_t milliseconds); 
extern void blink_function(uint16_t count, uint32_t delay_milliseconds);
//extern void 
void init_uart1(void);
void init_gpio(void);
void warning_led(void);
void uart_print(const char* string);
void sys_print_error(const char* message);

/*
   warning_led - функция которая будет использоваться для мигания в ситуациях когда у нас возникают ошибки 
   включает светодиод на 3 секунды и потом выключает его предупреждая что случилась ошибка.
*/
void warning_led(void)
{
   REGISTER_POINTER(GPIO_OUTPUT_SET_REGISTER) = (1 << 8);
   delay(3000);
   REGISTER_POINTER(GPIO_OUTPUT_CLEAR_REGISTER) = (1 << 8);
}
/*uart_print - функция которая печатает символы через UART0 в терминал для вывода ошибок*/
void uart_print(const char* string) 
{
    while (*string) 
    {
        // Ждем, если буфер почти полон
        while (((REGISTER_POINTER(UART_STATUS_REGISTER) >> 16) & 0xFF) >= 120) {}
        
        // Отправляем символ
        REGISTER_POINTER(UART_FIFO_REG) = *string++;
    }
}
#endif