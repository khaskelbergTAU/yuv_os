ENTRY(_start)

SECTIONS {
    . = 0x1M;

    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K){
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K){
        *(COMMON)
        *(.bss)
    }
}