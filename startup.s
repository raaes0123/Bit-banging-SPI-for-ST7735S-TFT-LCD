.section .vectors
vector_table:
    .word _stack_top    // Stack Pointer
    .word reset_handler

.section .text
.global reset_handler
.align 2
.type reset_handler, %function
reset_handler:
    b c_startup       
