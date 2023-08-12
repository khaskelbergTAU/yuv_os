global _start:function
global _load_gdt
global _load_idt
global _reload_segments
global _load_cr3
global level4table
global cause_interrupt

extern kernel_main
extern _init
extern __kernel_start
extern __kernel_end

%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)
%define NUM_PT_ENTRIES 512
%define CODE_SEG     0x0008
%define DATA_SEG     0x0010
%define PML4_ADDR 0x109000
%define PDPT_ADDR 0x10a000
%define PD_ADDR 0x10b000
%define PT_ADDR 0x10c000
%define KERNEL_VIRTUAL_BASE 0xFFFFFFFF80000000
%define KERNEL_PML4_ENTRY ((KERNEL_VIRTUAL_BASE >> 39) & 0b111111111)
%define KERNEL_PDPT_ENTRY ((KERNEL_VIRTUAL_BASE >> 30) & 0b111111111)
%define KERNEL_PD_ENTRY   ((KERNEL_VIRTUAL_BASE >> 21) & 0b111111111)
%define KERNEL_PT_ENTRY   ((KERNEL_VIRTUAL_BASE >> 12) & 0b111111111)
%define MULTIBOOT2_HEADER_MAGIC 0xe85250d6
%define MULTIBOOT_ARCHITECTURE_I386  0
%define MULTIBOOT_TAG_TYPE_END               0
%define MULTIBOOT_HEADER_TAG_END  0


HEADER_LENGTH equ header_end - header_start
CHECKSUM equ -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + HEADER_LENGTH)
section .multiboot

header_start:
dd MULTIBOOT2_HEADER_MAGIC
dd MULTIBOOT_ARCHITECTURE_I386
dd HEADER_LENGTH
dd CHECKSUM

dw MULTIBOOT_HEADER_TAG_END
dw 0
dd 8
header_end:


section .bss
KERNEL_STACK_SIZE equ 0x4000

align 64
stack_bottom:
resb KERNEL_STACK_SIZE
stack_top:

section .tables
align 0x1000
PML4:
times NUM_PT_ENTRIES dq 0

PDPT:
times NUM_PT_ENTRIES dq 0

PD:
times NUM_PT_ENTRIES dq 0

PT:
times NUM_PT_ENTRIES dq 0

section .data
align 0x1000
GDT:
.Null:
    dq 0x0000000000000000             ; Null Descriptor - should be present.
 
.Code:
    dq 0x00af9a000000ffff             ; 64-bit code descriptor (exec/read).
    dq 0x00af92000000ffff             ; 64-bit data descriptor (read/write).
 
ALIGN 4
    dw 0                              ; Padding to make the "address of the GDT" field aligned on a 4-byte boundary
 
.Pointer:
    dw $ - GDT - 1                    ; 16-bit Size (Limit) of GDT.
    dq GDT                            ; 32-bit Base Address of GDT. (CPU will zero extend to 64-bit)
ALIGN 4

idtr:
dw 0 ; For limit storage
dd 0 ; For base storage


section .bootloader
[BITS 32]

align 0x1000
boot_GDT:
.Null:
    dq 0x0000000000000000             ; Null Descriptor - should be present.
 
.Code:
    dq 0x00af9a000000ffff             ; 64-bit code descriptor (exec/read).
    dq 0x00af92000000ffff             ; 64-bit data descriptor (read/write).
 
ALIGN 4
    dw 0                              ; Padding to make the "address of the GDT" field aligned on a 4-byte boundary
 
.Pointer:
    dw $ - boot_GDT - 1                    ; 16-bit Size (Limit) of GDT.
    dq boot_GDT                            ; 32-bit Base Address of GDT. (CPU will zero extend to 64-bit)

IDT:
dw 0
dd 0

_start:
    ; set PML4 page table entries
    mov eax, PDPT_ADDR
    or eax, PAGE_PRESENT | PAGE_WRITE

    mov edx, PML4_ADDR
    mov [edx], eax
    lea edx, [edx + KERNEL_PML4_ENTRY * 8]
    mov [edx], eax

    ; set PDPT page table entries
    mov eax, PD_ADDR
    or eax, PAGE_PRESENT | PAGE_WRITE

    mov edx, PDPT_ADDR
    mov [edx], eax
    lea edx, [edx + KERNEL_PDPT_ENTRY * 8]
    mov [edx], eax

    ; set PD page table entries
    mov eax, PT_ADDR
    or eax, PAGE_PRESENT | PAGE_WRITE

    mov edx, PD_ADDR
    mov [edx], eax
    lea edx, [edx + KERNEL_PD_ENTRY * 8]
    mov [edx], eax


    xor eax, eax
    mov ecx, 512
    or eax, PAGE_PRESENT | PAGE_WRITE
    mov edx, PT_ADDR
pt_loop:

    mov [edx], eax
    lea ebx, [edx + KERNEL_PT_ENTRY * 8]
    mov [ebx], eax
    dec ecx
    add eax, 0x1000
    add edx, 8
    cmp ecx, 0
    jg pt_loop



    ; Disable IRQs
    mov al, 0xFF                      ; Out 0xFF to 0xA1 and 0x21 to disable all IRQs.
    out 0xA1, al
    out 0x21, al
 
    ; Enter long mode.
    mov eax, 10100000b                ; Set the PAE and PGE bit.
    mov cr4, eax
 
    mov edx, PML4_ADDR                      ; Point CR3 at the PML4.
    mov cr3, edx
 
    mov ecx, 0xC0000080               ; Read from the EFER MSR. 
    rdmsr    
 
    or eax, 0x00000100                ; Set the LME bit.
    wrmsr
 
    mov ebx, cr0                      ; Activate long mode -
    or ebx,0x80000001                 ; - by enabling paging and protection simultaneously.
    mov cr0, ebx                    
 
    mov ebx, boot_GDT.Pointer
    lgdt [ebx]               
    mov eax, DATA_SEG
    mov ss, eax
    mov gs, eax
    mov fs, eax
    mov ds, eax
    mov es, eax
 
    jmp 0x08:LONG_MODE

[BITS 64]
LONG_MODE:
    jmp _start_in_higher_half


[BITS 64]
section .text
_load_idt:
    push rbp
    mov rbp, rsp
    push rcx
    push rax
    mov rax, idtr
    mov cx, [rbp + 0xc]
    mov [rax], cx
    mov rcx, [rbp + 0x8]
    mov [rax + 2], rcx
    lidt [rax]
    pop rax
    pop rcx
    mov rsp, rbp
    pop rbp
    ret

_load_cr3:
    push rbp
    mov rbp, rsp
    push rcx
    mov rcx, [ebp + 0x8]
    mov cr3, rcx
    pop rcx
    mov rsp, rbp
    pop rbp
    ret

_start_in_higher_half:
    mov rsp, stack_top

    mov rax, PML4
    mov qword [rax], 0
    invlpg [0]


    call _init

    mov rax, PML4
    push rax
    call kernel_main

    ;cli
.loop:
    hlt
    jmp .loop