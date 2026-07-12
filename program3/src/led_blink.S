.section .text
    .global blink_function
    .type blink_function,@function

    .equ GPIO_OUT_W1TS_REG, 0x60004008 # макрос адреса регистра high 
    .equ GPIO_OUT_W1TC_REG, 0x6000400C # макрос адреса регистра low
    .equ LED_GPIO_BIT,      (1 << 8)   # GPIO8 для светодиода

blink_function:
    addi    sp, sp, -16
    sw      ra, 12(sp)
    sw      s0, 8(sp)
    sw      s1, 4(sp)
    mv      s0, a0      # параметр счетчика миганий  где а0 это uint16_t count в функции
    mv      s1, a1      # параметр задержки  где а1 это uint16_t delay в функции

blink_loop:
    beqz    s0, blink_done

    li      t0, GPIO_OUT_W1TC_REG
    li      t1, LED_GPIO_BIT
    sw      t1, 0(t0)   # устанавливаем GPIO8 в LOW "инвертированное состояние"
    mv      a0, s1
    call    delay_blink

    li      t0, GPIO_OUT_W1TS_REG
    li      t1, LED_GPIO_BIT
    sw      t1, 0(t0)   # устанавливаем GPIO8 в HIGH
    mv      a0, s1
    call    delay_blink

    addi    s0, s0, -1
    j       blink_loop

blink_done:
    lw      ra, 12(sp)
    lw      s0, 8(sp)
    lw      s1, 4(sp)
    addi    sp, sp, 16
    ret

    .global delay_blink
    .type delay_blink,@function
delay_blink:
    li      t2, 80000   # ну это примерно 1 миллисекунда так как при 160 МГц, 2 такта итерация
    mul     t2, t2, a0  # время задержки
delay_loop:
    addi    t2, t2, -1
    bnez    t2, delay_loop
    ret
    