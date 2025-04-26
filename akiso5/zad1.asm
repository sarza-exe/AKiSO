%include        'functions.asm'

SECTION .data
msg1        db      'Podaj liczbe: ', 0h
msg2        db      'Suma cyfr wynosi: ', 0h

SECTION .bss
innumber:   resb    255

SECTION .text
global _start

_start:
    mov     eax, msg1
    call    sprint 
    
    mov     edx, 255                ;buffer size
    mov     ecx, innumber
    mov     ebx, 0                  ;STDIN
    mov     eax, 3                  ;read
    int     0x80

    mov     eax, innumber           
    call    atoi                    ; change ascii to integer
    mov     ecx, 10                       
    mov     ebx, 0

digit_sum:
    mov     edx, 0
    div     ecx                     ; divide eax by ecx
    add     ebx, edx                ; add eax mod 10 to ebx
    cmp     eax, 0                  ; check if eax = 0
    jnz     digit_sum               ; if not, continue loop

continue:
    mov     eax, msg2
    call    sprint

    mov     eax, ebx                ; drukuj sume cyfr
    call    iprintLF

    call    quit