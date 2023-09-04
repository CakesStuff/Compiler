bits 64

global prints
prints:
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    pop rbx
    pop rsi
    push rbx
.loop:
    cmp byte [rsi], 0
    je .end
    syscall
    inc rsi
    jmp .loop
.end:
    ret