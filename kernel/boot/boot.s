global _start:function
global _load_gdt
global _load_idt
global _reload_segments
global _load_cr3

extern kernel_main
extern _init
extern __kernel_start
extern __boot_start
extern __kernel_end
extern __cxa_finalize

%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)
%define PAGE_SIZE       (1 << 7)
%define NUM_PT_ENTRIES 512
%define CODE_SEG     0x0008
%define DATA_SEG     0x0010
%define KERNEL_VIRTUAL_BASE 0xFFFFFFFF80000000
%define ENTRY(addr, off)  (((addr) >> (off)) & 0b111111111)
%define PML4_ENTRY(addr) (ENTRY(addr, 39))
%define PDPT_ENTRY(addr) (ENTRY(addr, 30))
%define KERNEL_PML4_ENTRY (PML4_ENTRY(KERNEL_VIRTUAL_BASE))
%define KERNEL_PDPT_ENTRY (PDPT_ENTRY(KERNEL_VIRTUAL_BASE))
%define MULTIBOOT2_HEADER_MAGIC 0xe85250d6
%define MULTIBOOT_ARCHITECTURE_I386  0
%define MULTIBOOT_TAG_TYPE_END               0
%define MULTIBOOT_HEADER_TAG_END  0
%define V2P(addr) (addr - KERNEL_VIRTUAL_BASE)


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
    mov esp, stack_top - KERNEL_VIRTUAL_BASE
    ; set PML4 page table entries
    mov eax, V2P(PDPT)
    or eax, PAGE_PRESENT | PAGE_WRITE

    ; this maps two 1GB huge pages for the kernel. one identity mapping, and another higher-half one. this is temporary.
    mov edx, V2P(PML4)
    mov [edx], eax
    lea edx, [edx + KERNEL_PML4_ENTRY * 8]
    mov [edx], eax

    xor eax, eax
    or eax, PAGE_PRESENT | PAGE_WRITE | PAGE_SIZE
    mov edx, V2P(PDPT)
    mov [edx], eax
    lea edx, [edx + KERNEL_PDPT_ENTRY * 8]
    mov [edx], eax

    ; Disable IRQs
    mov al, 0xFF                      ; Out 0xFF to 0xA1 and 0x21 to disable all IRQs.
    out 0xA1, al
    out 0x21, al
 
    ; Enter long mode.
    mov eax, 10110000b                ; Set the PAE, PGE and PSE bits.
    mov cr4, eax
 
    mov edx, V2P(PML4)                      ; Point CR3 at the PML4.
    mov cr3, edx
 
    mov ecx, 0xC0000080               ; Read from the EFER MSR. 
    rdmsr    
 
    or eax, 0x00000100                ; Set the LME bit.
    wrmsr
 
    mov eax, cr0                      ; Activate long mode -
    or eax,0x80000001                 ; - by enabling paging and protection simultaneously.
    mov cr0, eax                    
 
    mov eax, boot_GDT.Pointer
    lgdt [eax]               
    mov eax, DATA_SEG
    mov ss, eax
    mov gs, eax
    mov fs, eax
    mov ds, eax
    mov es, eax
 
    jmp CODE_SEG:LONG_MODE

[BITS 64]
LONG_MODE:
    jmp _start_in_higher_half


[BITS 64]
section .text
_load_idt: ; rdi - idt addr, si - size
    push rax
    mov rax, idtr
    mov [rax], si
    mov [rax + 2], rdi
    lidt [rax]
    pop rax
    ret

_load_gdt: ; rdi - gdt addr, si - size
    push rax
    mov rax, GDT.Pointer
    mov [rax], si
    mov [rax + 2], rdi
    lgdt [rax]
    pop rax
    ret

_start_in_higher_half:
    mov rsp, stack_top

    mov rax, PML4
    mov qword [rax], 0
    mov rax, PDPT
    mov qword [rax], 0
    invlpg [0]


    call _init


    mov rdi, PML4
    mov rsi, __boot_start + KERNEL_VIRTUAL_BASE
    mov rdx, __kernel_end
    mov rcx, KERNEL_VIRTUAL_BASE
    mov r8, rbx
    add r8, KERNEL_VIRTUAL_BASE
    call kernel_main
    sub rsp, 8
    mov qword [rsp], 0x0
 
    call __cxa_finalize
 
    add rsp, 8

    jmp hlt_loop
_reload_segments:
   pushfq
   pop rax
   and rax, 1111111111111111111111111111111111111111111111101011111011111111b
   push rax
   popfq
   ; Reload CS register containing code selector:
   push DATA_SEG
   push rsp
   pushfq
   push CODE_SEG
   push .reload_CS
   iretq
.reload_CS:
   pop rax
   ; Reload data segment registers:
   mov   AX, DATA_SEG 
   mov   DS, AX
   mov   ES, AX
   mov   FS, AX
   mov   GS, AX
   mov   SS, AX
   ret
hlt_loop:
    hlt
    jmp hlt_loop