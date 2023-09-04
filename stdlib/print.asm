bits 64

global print
print:
    mov rax, 1
    mov rdi, 1
    pop rbx
    pop rsi
    push rbx
    mov rdx, 13
    syscall
    ret