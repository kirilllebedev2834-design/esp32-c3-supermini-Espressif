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

void init_uart1(void)
{
    /*
        To initialize UARTn:
        • enable the clock for UART RAM by setting SYSTEM_UART_MEM_CLK_EN to 1;
        • enable APB_CLK for UARTn by setting SYSTEM_UARTn_CLK_EN to 1;
        • clear SYSTEM_UARTn_RST;
        • write 1 to UART_RST_CORE;
        • write 1 to SYSTEM_UARTn_RST;
        • clear SYSTEM_UARTn_RST;
        • clear UART_RST_CORE;
        • enable register synchronization by clearing UART_UPDATE_CTRL
    */
    
}

void init_gpio(void)
{
    // GPIO8 - светодиод
    *(volatile uint32_t*)(IO_MUX_CONF_REG_FOR_PIN_GPIO8) = (1 << 12);
    *(volatile uint32_t*)(GPIO_OUTPUT_ENABLE_REGISTER) |= (1 << 8);
    // GPIO9 - кнопка boot 
    *(volatile uint32_t*)(IO_MUX_CONF_REG_FOR_PIN_GPIO9) = (1 << 12);
    *(volatile uint32_t*)(IO_MUX_CONF_REG_FOR_PIN_GPIO9) |= (1 << 9);
}

void app_main(void) 
{
    
}