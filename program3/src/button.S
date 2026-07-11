.section .text
.globl button_function
.type button_function, @function

.equ GPIO_IN_REG, 0x60004004 # макрос адреса регистра входа
.equ BUTTON_GPIO_BIT, (1 << 2) # GPIO 2 для кнопки

button_function:
    addi    sp, sp, -16
    sw      ra, 12(sp)
    sw      s0, 8(sp)
    sw      s1, 4(sp)
    
    li      t0, GPIO_IN_REG
    lw      t1, 0(t0)   # читаем состояние GPIO2
    and     t1, t1, BUTTON_GPIO_BIT

    beqz    t1, button_pressed # если кнопка нажата (GPIO2 LOW), переходим к обработке нажатия
    j       button_done

button_pressed:
    li      a0, 1
    j       button_done
    
button_done:
    lw      ra, 12(sp)
    lw      s0, 8(sp)
    addi    sp, sp, 16
    ret