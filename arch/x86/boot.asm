ORG 0x7c00
BITS 16

%define PE_BIT (1 << 0)

start:
    cli
    mov ax, 0
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov sp, 0x7c00
    
    lgdt [GdtRegister]

    mov ebx, cr0
    or ebx, PE_BIT
    mov cr0, ebx

    jmp 0x08:ProtectedMode

GdtStart:
    null:
        dq 0

    code:
        dw 0xFFFF
        dw 0
        db 0
        db 0x9B
        db 0xCF
        db 0
    
    data:
        dw 0xFFFF
        dw 0
        db 0
        db 0x93
        db 0xCF
        db 0

GdtEnd:

GdtRegister:
    dw GdtEnd - GdtStart - 1
    dd GdtStart

[BITS 32]
ProtectedMode:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov edi, 0xB8000

    mov al, 'A'
    mov ah, 0x3F

    mov [edi], ax

    jmp hang

hang:
    hlt
    jmp hang

times 510 - ($ - $$) db 0
dw 0xAA55