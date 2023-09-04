bits 64

global print
print:
    mov rax, 1
    mov rdi, 1
    pop rsi
    mov rdx, 13
    syscall