#ifndef MACROSES_AND_OTHERS_FOR_ESP32
#define MACROSES_AND_OTHERS_FOR_ESP32

#include <stdint.h>

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
#define UART_UPDATE_REGISTER 0x600000A4  // Синхронизация регистров
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

#define PROGRAM2_ADDRESS 0x40380000
#define PROGRAM2_STACK_TOP 0x3FCDFFFF
#define GDMA_IN_STATE_CH0_REGISTER 0x6003F084
#define UART_BUFFER_SIZE 128

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
   SYSTEM_ERROR_DMA_STILL_RUNNING,
   SYSTEM_ERROR_INTERRUPTS_ACTIVE,
   SYSTEM_ERROR_BUFFER_OVERFLOW
} error_status_t; 


extern void delay(uint16_t milliseconds); 
extern void blink_function(uint16_t count, uint32_t delay_milliseconds);
extern void jump_to_program(uint32_t program_start_adress, uint32_t top_of_the_program_stack) __attribute__((noreturn));
error_status_t init_uart(void);
error_status_t init_gpio(void);
error_status_t check_jump(uint32_t program_start_adress, uint32_t top_of_the_program_stack);
void warning_led(void);
void uart_print(const char* string);
void sys_print_error(const char* message);

#endif