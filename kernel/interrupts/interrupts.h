#ifndef _KERNEL_INTERRUPTS_H
#define _KERNEL_INTERRUPTS_H

#include <stdint.h>
#include "video.h"
#include "serial.h"
#include "handlers.h"
#include "pic.h"
#define IDT_SIZE 256
namespace interrupts
{
    using irq_handlers::irq_num;
    const uint8_t pic_master_irq = 0x20;
    const uint8_t pic_slave_irq = 0x28;
    void timer_phase(int hz);

    enum gate_type_e
    {
        INTERRUPT = 0xe,
        TRAP = 0xf
    };
    struct __attribute__((packed)) gate_descriptor_t
    {
        uint16_t offset_low; // offset is the offset into the section of the interrupt service routine, in my case its just the address of it.
        uint16_t selector; // selector into the GDT, in my case just the 2nd entry in it since segmentation is cringe
        uint8_t ist : 3;
        uint8_t res : 5; // trash
        gate_type_e gate_type : 4;
        uint8_t zero : 1;
        uint8_t priv : 2; // privilege - 0 for kernel, 3 for user
        uint8_t present : 1;
        uint64_t offset_high : 48;
        uint32_t res2;
        gate_descriptor_t(uintptr_t isr_addr, gate_type_e gate_type) :
            offset_low(isr_addr & ((1  << 16) - 1)),
            selector(1 << 3),
            res(0),
            gate_type(gate_type),
            zero(0),
            priv(0),
            present(1),
            offset_high((isr_addr >> 16) & ((1ULL << 48) - 1)),
            res2(0) {};
        gate_descriptor_t() = default;
    };
    static_assert(sizeof(gate_descriptor_t) == 16);
    extern gate_descriptor_t IDT[IDT_SIZE];
    void init_interrupts();
    extern "C"
    {
        extern void _load_idt(gate_descriptor_t IDT[IDT_SIZE], uint32_t idt_size);
    }
    void sti();
    void cli();
} // namespace interrupts

#endif