#ifndef MACROSES_AND_OTHERS_FOR_ESP32
#define MACROSES_AND_OTHERS_FOR_ESP32

#include <stdint.h>

#define true ((_Bool)1)
#define false ((_Bool)0)
#define bool _Bool
#define REGISTER_POINTER(adress) (*(volatile uint32_t*)(adress))

// UART macroses for work from documentation:
#define UART_FIFO_REG          0x60000000  // Буфер приема/передачи (FIFO)
#define UART_CLKDIV_REG        0x60000014  // Делитель частоты (скорость)
#define UART_STATUS_REGISTER   0x6000001C
#define UART_CONF_REG          0x60000020  // Конфигурация кадра
#define UART_CLK_CONF_REG      0x60000078  // Настройка частоты ядра
#define UART_UPDATE_REGISTER   0x600000A4  // Синхронизация регистров
#define SYS_CLK_EN_REG         0x600C0010  // Включение тактирования периферии
#define SYS_RST_EN_REG         0x600C0018  // Сброс периферии
#define UART_BUFFER_SIZE       128

//GPIO and IO_MUX macroses for work from documentation:
#define GPIO_OUTPUT_SET_REGISTER           0x60004008 
#define GPIO_OUTPUT_CLEAR_REGISTER         0x6000400C
#define GPIO_OUTPUT_ENABLE_REGISTER        0x60004020
#define GPIO_OUTPUT_ENABLE_SET_REGISTER    0x60004024
#define GPIO_OUTPUT_ENABLE_CLEAR_REGISTER  0x60004028
#define IO_MUX_CONF_REG_FOR_PIN_GPIO9      0x60009028
#define IO_MUX_CONF_REG_FOR_PIN_GPIO8      0x60009024


#define GDMA_IN_STATE_CHA_REGISTER 0x6003F084

/*
   чтобы сделать правильно прыжок можно воспользоваться обычной загрузкой
   (или же Normal Flash Boot) поддерживает безопасную загрузку (Security Boot)
   загрузчик ПЗУ загружает программу из Flash в SRAM и выполняет её
   прыжок из-за этого станет более безопасным
   есть еще Прямая загрузка (Direct Boot) но она используется только для одной программы
   из выше перечисленного следует что нам нужно знать RAM и Flash адреса а также размер программы
*/ 
#define FLASH_INSTRUCTIONS_BUS 0x42000000

// 1. куда мы запишем вторую программу и ее размер
#define PROGRAM2_ADDRESS    0x40390000 // 0x403_8_0000 - адрес программы bootloader
#define PROGRAM2_FLASH      0x20000
#define PROGRAM2_SIZE       0x10000
#define PROGRAM2_STACK_TOP  0x3FCD0000

// 2. после прыжка нужно передать состояние периферии для этого нам надо RTC FAST Memory
// данные в этой памяти не могут быть перезаписаны самим МК
#define RTC_FAST_M 0x50000000 

// битовые маски UART и GPIO для того чтобы знать устройства настроены или нет
#define PERIPH_UART0_MASK (1 << 0)
#define PERIPH_GPIO_MASK  (1 << 1)
#define PERIPH_TIMER_MASK (1 << 2)

// макросы для TIMER 0
#define TIMER0_CONFIG_REGISTER    0x6001F000
#define TIMER0_CURR_VALUE_LOW     0x6001F004   // младшие 32 бита счетчика
#define TIMER0_CURR_VALUE_HIGH    0x6001F008   // старшие 22 бита счетчика
#define TIMER0_UPDATE_REGISTER    0x6001F00C
#define TIMER0_CLKG_REGISTER      0x6001F0FC
#define TIMER0_LOADLOW_REGISTER   0x6001F018
#define TIMER0_LOADHIGH_REGISTER  0x6001F01C
#define TIMER0_LOAD_REGISTER      0x6001F020     
#define SYSTEM_PERIP_CLK_EN0      0x600C0010
#define SYSTEM_PERIP_RST_EN0      0x600C0018
#define SYSTEM_TIMERGROUP_CLK_EN  (1 << 2)
#define SYSTEM_TIMERGROUP_RST     (1 << 2)
#define TIMER0_ENABLE             (1 << 0)
#define TIMER0_INCREASE           (1 << 1)
#define TIMER0_DIVIDER            (79 << 4)   /* [28:4], деление на 80 */

// структура которая помогает перенести периферию после прыжка
typedef struct 
{
   uint32_t special_num;   // уникальное число для прыжка 
   uint32_t periph_mask;   // что настроенно
   uint32_t uart_clkdiv;   // скорость бод UART потому что при прыжке регистр может сброситься и перенастроено
   uint32_t gpio_out;      // состояние выходов GPIO
   uint32_t gpio_enable;   // какие пины настроены на выход
   uint32_t timer_config;  // конфигурация таймера
} __attribute__((packed)) periph_state_t;
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
   SYSTEM_ERROR_GDMA_STILL_RUNNING,
   SYSTEM_ERROR_INTERRUPTS_ACTIVE,
   SYSTEM_ERROR_BUFFER_OVERFLOW,
} error_status_t; 


extern void delay(uint16_t milliseconds); 
extern void blink_function(uint16_t count, uint32_t delay_milliseconds);
extern void jump_to_program(uint32_t program_start_adress, uint32_t top_of_the_program_stack) __attribute__((noreturn));
static inline periph_state_t periph_get(void);
error_status_t check_timer(void);
error_status_t init_uart(void);
error_status_t init_gpio(void);
error_status_t check_jump(uint32_t program_start_adress, uint32_t top_of_the_program_stack);
error_status_t uart_print(const char* string);
void init_timer(void);
static void load_from_flash(uint32_t program_flash, uint32_t program_address, uint32_t program_size);
void uart_flush_tx(void);
void warning_led(void);
void sys_print_error(const char* message);

#endif