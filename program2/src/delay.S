.section .text
.global delay
.type delay,@function

.equ millisecond, 80000 # ~1 миллисекунда при 160 МГц, 2 такта на итерацию

delay:
    li      t2, millisecond
    mul     t2, t2, a0       # t2 = millisecond * count (a0 = uint16_t count)
delay_loop:
    addi    t2, t2, -1
    bnez    t2, delay_loop
    ret