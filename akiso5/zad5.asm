
; nasm -fbin zad5.asm
; qemu-system-i386 -drive format=raw,file=zad5

bits 16                 ; the code is for a 16-bit cpu
; real vs protected mode
; real mode uses 16-bit registers and instructions (e.g., AX, BX).
; real mode is limited to 1 MB of memory
org 0x7c00              ; set the origin of the program to the memory address of Bootloaders

boot:
    mov ax, 0x2401
    int 0x15            ; enable advanced BIOS features
    mov ax, 0x13        ; wlaczenie trybu graficznego 320x200
    int 0x10            ; BIOS routines for video services
    cli                 ; disable hardware interrupts
    lgdt [gdt_pointer]  ; ustawienie tablicy GDT - Global Descriptor Table 
    mov eax, cr0        ; move CR0 control register to eax
    or eax,0x1          ; change PE bit to protected mode
    mov cr0, eax        ; enable protected mode
    jmp CODE_SEG:boot2
;Global Descriptor Table defines memory segments in prot. mode
gdt_start:              ; tablica GDT
    dq 0x0              ; First entry in GDT is always 0
gdt_code:               ; code segment descriptor
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_data:               ; data segment descriptor
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:                ; Marks the end of the GDT table
gdt_pointer:
    dw gdt_end - gdt_start ;Specifies the size of the GDT 
    dd gdt_start           ;Specifies the linear base address of the GDT

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

%include    'functions.asm'

bits 32
boot2:
    ; Initialize data segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Initialize video memory pointer
    mov ebx, 0xa0000       ; Start of video memory
    mov cx,200          ; 200 lini ekranu

    ; Screen dimensions and ranges
    mov eax, 320           ; Screen width
    mov [screen_width], eax
    mov eax, 200           ; Screen height
    mov [screen_height], eax

.loop2:
    mov dx,320          ; 320 piksli na linie
.loop1:
    mov byte [ebx], cl  ; determines pixel color
    push ebx
    push ecx
    push edx


    pop edx
    pop ecx
    pop ebx
    add ebx,1           ; advance EBX to next pixel
    dec dx
    jne .loop1          ; if dx != 0 jump to .loop1
    loop .loop2         ; if cx != zero jump to .loop2 and cx--

halt:
    cli
    hlt

; Variables and constants
screen_width dd 320
screen_height dd 200
c_re dd 1
c_im dd 1
imag_min dd -1.25
imag_max dd 1.25
max_iterations dd 32

times 510 - ($-$$) db 0 ; ensures the assembled code is exactly 510 bytes long
dw 0xaa55
