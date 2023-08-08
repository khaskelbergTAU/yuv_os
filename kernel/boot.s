global _start
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
%define PDPT_ADDR 0x110000
%define PD_ADDR 0x111000
%define PT_ADDR 0x112000
%define KERNEL_VIRTUAL_BASE 0xFFFFFFFF80000000
%define KERNEL_PML4_ENTRY ((KERNEL_VIRTUAL_BASE >> 39) & 0b111111111)
%define KERNEL_PDPT_ENTRY ((KERNEL_VIRTUAL_BASE >> 30) & 0b111111111)
%define KERNEL_PD_ENTRY   ((KERNEL_VIRTUAL_BASE >> 21) & 0b111111111)
%define KERNEL_PT_ENTRY   ((KERNEL_VIRTUAL_BASE >> 12) & 0b111111111)


ALIGN_FLAG equ 1<<0
MEMINFO equ 1<<1
FLAGS equ ALIGN_FLAG | MEMINFO
MAGIC equ 0x1badb002
CHECKSUM equ -(MAGIC + FLAGS)
KERNEL_STACK_SIZE equ 0x4000



section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM
[BITS 64]


section .bss
align 16
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
    dq 0x00209A0000000000             ; 64-bit code descriptor (exec/read).
    dq 0x0000920000000000             ; 64-bit data descriptor (read/write).
 
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


IDT:
dw 0
dd 0

_start:
    ; set PML4 page table entries
    mov eax, PDPT_ADDR
    or eax, PAGE_PRESENT | PAGE_WRITE

    mov edx, PML4_ADDR
    mov [edx], eax
    lea edx, [edx + KERNEL_PML4_ENTRY]
    mov [edx], eax

    ; set PDPT page table entries
    mov eax, PD_ADDR
    or eax, PAGE_PRESENT | PAGE_WRITE

    mov edx, PDPT_ADDR
    mov [edx], eax
    lea edx, [edx + KERNEL_PDPT_ENTRY]
    mov [edx], eax

    ; set PD page table entries
    mov eax, PT_ADDR
    or eax, PAGE_PRESENT | PAGE_WRITE

    mov edx, PD_ADDR
    mov [edx], eax
    lea edx, [edx + KERNEL_PD_ENTRY]
    mov [edx], eax


        ; Disable IRQs
    mov al, 0xFF                      ; Out 0xFF to 0xA1 and 0x21 to disable all IRQs.
    out 0xA1, al
    out 0x21, al
 
    lidt [IDT]                        ; Load a zero length IDT so that any NMI causes a triple fault.
 
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
 
    lgdt [GDT.Pointer - KERNEL_VIRTUAL_BASE]                ; Load GDT.Pointer defined below.
 
    jmp CODE_SEG:(_start_in_higher_half - KERNEL_VIRTUAL_BASE)             ; Load CS with 64 bit segment and flush the instruction cache

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
    mov dword [rax], 0
    invlpg [0]


    call _init

    mov rax, PML4
    push rax
    call kernel_main

    ;cli
.loop:
    hlt
    jmp .loop



[BITS 64]      
LongMode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
 
    ; Blank out the screen to a blue color.
    mov edi, 0xB8000
    mov rcx, 500                      ; Since we are clearing uint64_t over here, we put the count as Count/4.
    mov rax, 0x1F201F201F201F20       ; Set the value to set the screen to: Blue background, white foreground, blank spaces.
    rep stosq                         ; Clear the entire screen. 
 
    ; Display "Hello World!"
    mov edi, 0x00b8000              
 
    mov rax, 0x1F6C1F6C1F651F48    
    mov [edi],rax
 
    mov rax, 0x1F6F1F571F201F6F
    mov [edi + 8], rax
 
    mov rax, 0x1F211F641F6C1F72
    mov [edi + 16], rax
 
 loops:
    jmp loops                     ; You should replace this jump to wherever you want to jump to.
