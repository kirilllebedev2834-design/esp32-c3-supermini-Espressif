.section .text
.global jump_to_program
.type jump_to_program, @function

jump_to_program:
    # a0 = program_start_adress PROGRAM2_ADDRESS 0x40380000 (Program2)
    # a1 = top_of_the_program_stack PROGRAM2_STACK_TOP 0x3FCDFFFF  (Program2)

    csrci mstatus, 8        # запрещает прерывания
    fence.i                 # синхронизирует кэш и конвейер 
    csrw mtvec, zero        # сбрасывает вектор прерываний bootloader
    mv sp, a1               # переключает указатель стека
    jr a0                   # прыжок в Program2

1:  j 1b                    