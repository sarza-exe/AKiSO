%include        'functions.asm'

SECTION .data
    msg_err     db "Program wymaga 1 liczby w systemie 10", 0h
    msg_nl      db 0Ah

SECTION .text
global  _start
 
_start:
 
    pop     ecx             ; first value on the stack is the number of arguments
    pop     edx             ; second value is the program name
    sub     ecx, 1          ; decrease ecx by 1 (number of arguments without program name)

    cmp     ecx, 1
    jne     wrongNumOfArgs

    pop     eax             ; pop the next argument off the stack
    call    atoi            ; change ascii into number

    mov     ebx, 16         ; we divide by 16 for a hex system
    mov     ecx, 0
    mov     edx, 0

    cmp     eax, 0          ; if the number is 0 then we would get our core dumped
    jnz     convertLoop     ; if not 0 jump to convertloop

zero:
    inc     ecx
    mov     edx, 48
    push    edx

convertLoop:
    cmp     eax, 0
    jz      printHex
    inc     ecx
    mov     edx, 0
    div     ebx
    cmp     edx, 9
    jle     number
    cmp     edx, 9
    jg      letter 
    
number:
    add     edx, 48
    push    edx
    jmp     convertLoop

letter:
    add     edx, 55
    push    edx
    jmp     convertLoop

printHex:
    dec     ecx             ; count down each byte that we put on the stack
    mov     eax, esp        ; mov the stack pointer into eax for printing
    call    sprint          ; call our string print function
    pop     eax             ; remove last character from the stack to move esp forward
    cmp     ecx, 0          ; have we printed all bytes we pushed onto the stack?
    jnz     printHex        ; jump is not zero to the label printLoop

end:
    mov     eax, msg_nl
    call    sprint
    call    quit
 
wrongNumOfArgs:
    mov     eax, msg_err
    call    sprintLF
    call    quit