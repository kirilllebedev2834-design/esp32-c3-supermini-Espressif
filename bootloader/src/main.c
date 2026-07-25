/*
    без библиотек для ESP но можно стандартные библиотеки
    прыжки надо делать без OTA - механизма
    есть смысл модульным подходом идти тк очень много кода будет для инициализации перифирии
    использовать С для работы с битами, аттрибуты, специальные операторы, volatile, препроцессы
    по возможности использовать ассемблер риск-5
    код пишется голыми руками буквально
    работа с компилятором и подключенным устройством
    нужно для написания кода: архитектура ЕСП32 С3, работа с памятью, карта памяти, работа с регистрами микроконтроллера
    самое главное: "общение" ПК с МК

    задание:
    Загрузчик (Bootloader)
    Функции:

    1. Инициализация периферийных устройств ESP32 (GPIO, UART и др.).
    2. Переход к выполнению второй программы (Program2) после инициализации.
    Требования:

    а) Использовать выделенную область памяти для загрузчика. 
    б) Обеспечить корректное переключение до Program2.
    в) Настроить UART с параметрами: [указать скорость, например, 115200 бод].
    г) Использовать светодиод для того чтобы указать что инициализация прошла успешно и мы в загрузчике
    д) Обработка ошибок: надёжная обработка возможных ошибок (например, неудачная инициализация периферии).  
    
*/
#include "macrosandother.h"


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

/* sys_error_print - функция для сообщения об ошибке */
void sys_error_print(const char* message)
{
    uart_print("Ошибка!\n");
    uart_print(message);
}

/*
Инициализация UARTn
Для инициализации UARTn:

• включите тактирование RAM UART, установив SYSTEM_UART_MEM_CLK_EN в 1;
• включите APB_CLK для UARTn, установив SYSTEM_UARTn_CLK_EN в 1;
• сбросьте SYSTEM_UARTn_RST;
• запишите 1 в UART_RST_CORE;
• запишите 1 в SYSTEM_UARTn_RST;
• сбросьте SYSTEM_UARTn_RST;
• сбросьте UART_RST_CORE;
• включите синхронизацию регистров, сбросив UART_UPDATE_CTRL.

Настройка связи UARTn

Для настройки связи UARTn:
• дождитесь, пока UART_REG_UPDATE станет равным 0, что указывает на завершение последней синхронизации;
• настройте статические регистры (если таковые имеются) в соответствии с разделом 26.5.1.2;
• выберите источник тактового сигнала через UART_SCLK_SEL;
• настройте коэффициент деления делителя через UART_SCLK_DIV_NUM, UART_SCLK_DIV_A и UART_SCLK_DIV_B;
• настройте скорость передачи (боудрейт) через UART_CLKDIV и UART_CLKDIV_FRAG;
• настройте длину данных через UART_BIT_NUM;
• настройте контроль чётности (нечётный или чётный) через UART_PARITY_EN и UART_PARITY;
• дополнительные шаги в зависимости от приложения ...
• синхронизируйте настроенные значения с доменом Core Clock, записав 1 в UART_REG_UPDATE.
*/
error_status_t init_uart(void)
{
    REGISTER_POINTER(SYS_CLK_EN_REG) |= (1 << 24) | (1 << 2);

    REGISTER_POINTER(SYS_RST_EN_REG) &= ~(1 << 2);

    REGISTER_POINTER(UART_CLK_CONF_REG) |= (1 << 23);

    REGISTER_POINTER(SYS_RST_EN_REG) |= (1 << 2);

    REGISTER_POINTER(SYS_RST_EN_REG) &= ~(1 << 2);

    REGISTER_POINTER(UART_CLK_CONF_REG) &= ~(1 << 23);

    REGISTER_POINTER(UART_UPDATE_REGISTER) &= ~(1 << 0);

    uint32_t timeout = 10000000;
    // • дождитесь, пока UART_REG_UPDATE станет равным 0 (завершение синхронизации)
    while ((REGISTER_POINTER(UART_UPDATE_REGISTER) & (1 << 0)) != 0) 
    {
        timeout--;
        if(timeout==0)
        {
            return SYSTEM_TIMEOUT;
        }
    }

    // • выберите источник тактового сигнала APB_CLK 80 МГц (биты 21:20 = 01),
    //   а также включите тактирование TX/RX (биты 25, 24, 22) и базовый делитель (биты 19:12 = 1)
    REGISTER_POINTER(UART_CLK_CONF_REG) = (1 << 25) | (1 << 24) | (1 << 22) | (1 << 20) | (1 << 12);

    // • настройте скорость передачи (115200 бод при частоте APB 80 МГц ( 80млн/115200 ~~ 694,(4) ): по формуле:
    //итоговая скорость передачи равна
    //INPUT_FREQ / (UART_CLKDIV + UART_CLKDIV_FRAG / 16)
    //где INPUT_FREQ — частота источника тактового сигнала ядра UART. целая часть 694, дробная 7)
    // • записываем в UART_CLKDIV (биты [23:4]) и UART_CLKDIV_FRAG (биты [3:0])
    REGISTER_POINTER(UART_CLKDIV_REG) = (694 << 4) | 7;

    // • настройте длину данных 8 бит (значение 11b в битах [3:2]) и отключите четность
    REGISTER_POINTER(UART_CONF_REG) = (3 << 2);

    // • синхронизируйте настроенные значения с доменом Core Clock, записав 1 в UART_REG_UPDATE (бит 0)
    REGISTER_POINTER(UART_UPDATE_REGISTER) |= (1 << 0);
    return SYSTEM_IS_OK;
}
/* переписал с UART1 на UART0 потому что UART0 тесносвязан с USB-JTAG из-за чего только с UART0 можно работать с терминалом*/


error_status_t init_gpio(void)
{
    // GPIO8 - светодиод
    REGISTER_POINTER(IO_MUX_CONF_REG_FOR_PIN_GPIO8) = (1 << 12);
    REGISTER_POINTER(GPIO_OUTPUT_ENABLE_REGISTER) |= (1 << 8);

    // проверка
    if( (REGISTER_POINTER(IO_MUX_CONF_REG_FOR_PIN_GPIO8) & (1 << 12)) == 0){ return SYSTEM_ERROR_INIT_FAILED; }
    if( (REGISTER_POINTER(GPIO_OUTPUT_ENABLE_REGISTER) & (1 << 8)) == 0){ return SYSTEM_ERROR_INIT_FAILED; }

    // GPIO9 - кнопка boot 
    uint32_t gpio_9_mask = (1 << 12) | (1 << 9) | (1 << 8) ;
    REGISTER_POINTER(IO_MUX_CONF_REG_FOR_PIN_GPIO9) = gpio_9_mask;
    if( (REGISTER_POINTER(IO_MUX_CONF_REG_FOR_PIN_GPIO9) & gpio_9_mask) == gpio_9_mask){ return SYSTEM_ERROR_INIT_FAILED; }

    return SYSTEM_IS_OK;
}

/* 
    check_jump - функция для проверки прыжка. 
    Проверяет, что по адресу лежит рабочий код, а не стертая память (0xFFFFFFFF).
    Проверяет, остановлен ли GDMA (чтобы он не писал в память во время прыжка)
    После всех проверок совершается прыжок в программу на указанный адрес.
*/
error_status_t check_jump(uint32_t program_start_adress, uint32_t top_of_the_program_stack)
{
    uint32_t mstatus, bit_mask_for_gdma = 0x00FFFFFF ;
    if (REGISTER_POINTER(program_start_adress) == 0xFFFFFFFF) return SYSTEM_ERROR_MEMORY_EMPTY_PROGRAM;
        
    __asm__ volatile("csrr %0, mstatus" : "=r"(mstatus));
    if (mstatus & 0x08) return SYSTEM_ERROR_INTERRUPTS_ACTIVE; // Бит MIE (Machine Interrupt Enable)
    if (REGISTER_POINTER(GDMA_IN_STATE_CH0_REGISTER) & bit_mask_for_gdma) return SYSTEM_ERROR_DMA_STILL_RUNNING;
    
    return SYSTEM_IS_OK;
}

void app_main(void) 
{
   error_status_t uart_conf = init_uart();
   if(uart_conf != SYSTEM_IS_OK)
   {
        warning_led();
        sys_error_print("Ошибка инициализации UART");
   }

   error_status_t gpio_conf = init_gpio();
   if(gpio_conf != SYSTEM_IS_OK)
   {
        warning_led();
        sys_error_print("Ошибка инициализации GPIO");
   }

   blink_function(1, 2000);
   error_status_t jump = check_jump(PROGRAM2_ADDRESS, PROGRAM2_STACK_TOP);
   if(jump == SYSTEM_IS_OK)
   {
        jump_to_program(PROGRAM2_ADDRESS, PROGRAM2_STACK_TOP);

        sys_print_error("Прыжок в другую программу не выполнен");
   }
   else
   {
        sys_error_print("Прыжок не прошел проверку");
   }
}
