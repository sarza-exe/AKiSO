%include    'functions.asm'

SECTION .data
    msg_err     db "Program wymaga podania 9 elementow macierzy!", 0h
    msg_sum     db "Suma elementow: ", 0h
    msg_diag    db "Suma przekatnej: ", 0h

SECTION .text
global  _start
 
_start:
    pop     ecx             ; first value on the stack is the number of arguments
    pop     edx             ; second value on the stack is the program name (discarded when we initialise edx)
    sub     ecx, 1          ; decrease ecx by 1 (number of arguments without program name)

    cmp     ecx, 9
    jne     wrongNumOfArgs  ; if there are not 9 args print error

    mov     edx, 0          ; edx - suma elementow
    mov     ebx, 0          ; ebx - suma przekatnej
 
nextArg:
    cmp     ecx, 0h         ; check to see if we have any arguments left
    jz      noMoreArgs      ; if zero flag is set jump to noMoreArgs label (jumping over the end of the loop)
    pop     eax             ; pop the next argument off the stack
    call    atoi            ; convert our ascii string to decimal integer

    add     edx, eax

    cmp     ecx, 1          ; indeks 0 -> pierwszy element
    je      add_to_diag
    cmp     ecx, 5          ; indeks 4 -> środkowy element
    je      add_to_diag
    cmp     ecx, 9          ; indeks 8 -> ostatni element
    je      add_to_diag
    jmp     skip_diag       ; Nie dodawaj do przekątnej

add_to_diag:
    add     ebx, eax

skip_diag:
    dec     ecx             ; decrease ecx (number of arguments left) by 1
    jmp     nextArg         ; jump to nextArg label
 
noMoreArgs:
    mov     eax, msg_sum
    call    sprint
    mov     eax, edx
    call    iprintLF 

    mov     eax, msg_diag
    call    sprint
    mov     eax, ebx 
    call    iprintLF 

    call    quit

wrongNumOfArgs:
    mov     eax, msg_err
    call    sprintLF

    call    quit