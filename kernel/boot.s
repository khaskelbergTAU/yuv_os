global _start
global _enable_paging
global _load_gdt
global _reload_segments
global _load_cr3
global level4table

extern kernel_main
extern _init

ALIGN_FLAG equ 1<<0
MEMINFO equ 1<<1
FLAGS equ ALIGN_FLAG | MEMINFO
MAGIC equ 0x1badb002
CHECKSUM equ -(MAGIC + FLAGS)
KERNEL_STACK_SIZE equ 0x4000
KERNEL_VIRTUAL_BASE equ 0xc0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)



section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM


section .bss
align 16
stack_bottom:
resb KERNEL_STACK_SIZE
stack_top:



section .data
align 0x1000
level4table:
dd 0x83
times (KERNEL_PAGE_NUMBER - 1) dd 0
dd 0x83
times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

gdtr:
dw 0 ; For limit storage
dd 0 ; For base storage



section .bootloader

_start:

    mov ecx, (level4table - KERNEL_VIRTUAL_BASE)
    mov cr3, ecx


    mov eax, cr4
    or eax, 0x10
    mov cr4, eax

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

    jmp _start_in_higher_half

section .text


_load_gdt:
    push ebp
    mov ebp, esp
    push ecx
    mov cx, [ebp + 0xc]
    mov [gdtr], cx
    mov ecx, [ebp + 0x8]
    mov [gdtr + 2], ecx
    lgdt [gdtr]
    pop ecx
    mov esp, ebp
    pop ebp
    ret


_reload_segments:
   ; Reload CS register containing code selector:
   jmp   0x08:.reload_CS ; 0x08 is code segment
.reload_CS:
   ; Reload data segment registers:
   mov   AX, 0x10 ; 0x10 is data segment
   mov   DS, AX
   mov   ES, AX
   mov   FS, AX
   mov   GS, AX
   mov   SS, AX
   ret

_load_cr3:
    push ebp
    mov ebp, esp
    push ecx
    mov ecx, [ebp + 0x8]
    mov cr3, ecx
    pop ecx
    mov esp, ebp
    pop ebp
    ret

_start_in_higher_half:
    mov esp, stack_top

    mov dword [level4table], 0
    invlpg [0]


    call _init

    push level4table
    call kernel_main

    cli
    hlt
.loop:
    jmp .loop
