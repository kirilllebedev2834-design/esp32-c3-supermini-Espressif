#ifndef MACROSES_AND_OTHERS_FOR_ESP32
#define MACROSES_AND_OTHERS_FOR_ESP32

#include <stdint.h>

#define true ((_Bool)1)
#define false ((_Bool)0)
#define bool _Bool
#define REGISTER_POINTER(adress) (*(volatile uint32_t*)(adress))

//GPIO and IO_MUX macroses for work from documentation:
#define GPIO_OUTPUT_SET_REGISTER 0x60004008 
#define GPIO_OUTPUT_CLEAR_REGISTER 0x6000400C
#define GPIO_OUTPUT_ENABLE_REGISTER 0x60004020
#define GPIO_OUTPUT_ENABLE_SET_REGISTER 0x60004024
#define GPIO_OUTPUT_ENABLE_CLEAR_REGISTER 0x60004028
#define GPIO_NUM_9 9

#define IO_MUX_CONF_REG_FOR_PIN_GPIO9 0x60009028
#define IO_MUX_CONF_REG_FOR_PIN_GPIO8 0x60009024

#define PROGRAM3_ADDRESS_FROM_FLASH 0x0
#define PROGRAM3_STACK_TOP 0x0
#define PROGRAM3_ADDRESS_FROM_RAM 0x0
#define PROGRAM3_SIZE 0x1

#define PAUSE 100
#define BLINK_TIME 300
#define RTC_FAST_M 0x50000000
#define SPECIAL_NUMBER 0xABCDABCD
typedef uint8_t byte;

// перечесления ошибок 
typedef enum
{
   SYSTEM_IS_OK = 0,
   SYSTEM_TIMEOUT,
   SYSTEM_INVALID_ARG,
   SYSTEM_INVALID_ADRESS,
   SYSTEM_ERROR_INIT_FAILED,
   SYSTEM_ERROR_MEMORY,
   SYSTEM_ERROR_MEMORY_EMPTY_PROGRAM,
   SYSTEM_INVALID_JUMP_TO_PROGRAM,
   SYSTEM_ERROR_GDMA_STILL_RUNNING,
   SYSTEM_ERROR_INTERRUPTS_ACTIVE,
   SYSTEM_ERROR_BUFFER_OVERFLOW
} error_status_t; 

typedef struct 
{
   uint32_t special_num;   // уникальное число для прыжка 
   uint32_t periph_mask;   // что настроенно
   uint32_t uart_clkdiv;   // скорость бод UART потому что при прыжке регистр может сброситься и перенастроено
   uint32_t gpio_out;      // состояние выходов GPIO
   uint32_t gpio_enable;   // какие пины настроены на выход
   uint32_t timer_config;  // конфигурация таймера
} __attribute__((packed)) periph_state_t;

extern void delay(uint32_t milliseconds); 
extern void blink_function(uint32_t delay_milliseconds_for_light);
extern void jump_to_program(uint32_t program_start_adress, uint32_t top_of_the_program_stack) __attribute__((noreturn));
extern bool button_function(uint32_t gpio_num);
static inline periph_state_t* periph_get(void);
error_status_t check_jump(uint32_t program_start_adress, uint32_t top_of_the_program_stack);
error_status_t uart_print(const char* string);
void waiting_button_for_jump(void);
void uart_flush_tx(void);
void warning_led(void);
void sys_print_error(const char* message);

#endif