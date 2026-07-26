.section .text
    .global blink_function
    .type blink_function,@function

    .equ GPIO_OUT_W1TS_REG, 0x60004008 # макрос адреса регистра high 
    .equ GPIO_OUT_W1TC_REG, 0x6000400C # макрос адреса регистра low
    .equ LED_GPIO_BIT,      (1 << 8)   # GPIO8 для светодиода

blink_function:
    addi    sp, sp, -16
    sw      ra, 12(sp)
    
    li      t0, GPIO_OUT_W1TC_REG   # { включаем светодиод
    li      t1, LED_GPIO_BIT        # {
    sw      t1, 0(t0)   # устанавливаем GPIO8 в LOW "инвертированное состояние"

    call    delay_for_blink         # { горит столько времени

    li      t0, GPIO_OUT_W1TS_REG   # { выключаем светодиод
    li      t1, LED_GPIO_BIT        # { 
    sw      t1, 0(t0)   # устанавливаем GPIO8 в HIGH
 
    lw      ra, 12(sp)
    addi    sp, sp, 16
    ret

    .global delay_for_blink
    .type delay_for_blink,@function
delay_for_blink:
    li      t2, 80000   # ну это примерно 1 миллисекунда так как при 160 МГц, 2 такта итерация
    mul     t2, t2, a0  # время задержки
delay_loop:
    addi    t2, t2, -1
    bnez    t2, delay_loop
    ret
    