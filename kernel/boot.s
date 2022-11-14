.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1badb002
.set CHECKSUM, -(MAGIC + FLAGS)
.set KERNEL_STACK_SIZE, 0x4000
.set KERNEL_VIRTUAL_BASE, 0xc0000000
.set KERNEL_PAGE_NUMBER, (KERNEL_VIRTUAL_BASE >> 22)



.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


.section .bss
.align 16
stack_bottom:
.skip KERNEL_STACK_SIZE
stack_top:

.align 0x1000
level4table:
.skip 0x1000







.section .text

.global _enable_paging
.type _enable_paging, @function
_enable_paging:
    push %ebp
    mov %esp, %ebp
    push %eax

    mov %cr0, %eax
    or %eax, 0x80000001
    mov %eax, %cr0

    mov %cr4, %eax
    or %eax, 0x10
    mov %eax, %cr4


    pop %eax
    mov %ebp, %esp
    pop %ebp
    ret


.global _load_cr3
.type _load_cr3, @function
_load_cr3:
    push %ebp
    mov %esp, %ebp
    push %eax

    mov 8(%esp), %eax
    mov %eax, %cr3

    pop %eax
    mov %ebp, %esp
    pop %ebp
    ret

.global _start
.type _start, @function
_start:
    mov $stack_top, %esp

    push level4table
    call map_kernel

    ;push $level4table - 0xc0000000
    ;call _load_cr3


    ;call _init

    call kernel_main

    cli
    hlt
1:  jmp 1b

.size _start, . - _start

