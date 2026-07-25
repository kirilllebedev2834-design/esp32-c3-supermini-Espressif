/*
    задание:
    Вторая программа (Program2)
    Функции:

    1. Моргание светодиодом два раза с интервалом в 500 мс между миганиями.
    2. Ожидание нажатия кнопки для перехода к третьей программе (Program3).
    3. Мигание светодиодом раз в секунду (сообщает о работе).
    Требования:

    а) Использовать GPIO для управления светодиодом и чтения состояния кнопки.
    б) Реализовать устойчивую обработку нажатий кнопки.
    в) После подтверждения нажатия кнопки перейти к Program3.
    г) Каждую секунду мигает светодиод показывая о работе во второй программе 
    д) Обработка ошибок: надёжная обработка возможных ошибок
*/
#include "macrosandotherprogram2.h"

void waiting_button_for_jump(void)
{
    static const byte gpio_num = GPIO_NUM_9;
    bool is_button_press = (button_function(1 << gpio_num) != 0);
    while(!is_button_press)
    {
        blink_function(BLINK_TIME);
        for(byte i = 0; i < 6; i++)
        {
            if(button_function(1 << gpio_num) != 0) { return; }
            delay(PAUSE);
        }  
    }
}

void app_main() 
{
    for(byte i = 0; i < 2; i++){ blink_function(500); }
    waiting_button_for_jump();

}