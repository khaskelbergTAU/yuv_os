#include "interrupts.h"
namespace interrupts
{
    using namespace irq_handlers;
    gate_descriptor_t IDT[IDT_SIZE]{};
    static void register_interrupt(uint32_t index, void (*handler)(interrupt_frame_t *))
    {
        IDT[index] = gate_descriptor_t(reinterpret_cast<uintptr_t>(handler), gate_type_e::INTERRUPT_32);
    }
    static void register_trap(uint32_t index, void (*handler)(interrupt_frame_t *, uint32_t))
    {
        IDT[index] = gate_descriptor_t(reinterpret_cast<uintptr_t>(handler), gate_type_e::TRAP_32);
    }
    void init_interrupts()
    {
        register_trap(DIV_ERROR, handle_divide_by_zero);
        register_interrupt(TIMER, handle_timer);
        register_interrupt(KEYBOARD, handle_keyboard);
        register_trap(DOUBLE_FAULT, handle_double_fault);
        pic::remap(pic_master_irq, pic_slave_irq);
        pic::unmask(TIMER - pic_master_irq);
        pic::unmask(KEYBOARD - pic_master_irq);
        timer_phase(1);
        _load_idt(IDT, sizeof(IDT) - 1);
        sti();
    }
    void timer_phase(int hz)
    {
        using namespace inline_asm;
        int divisor = 1193180 / hz;     /* Calculate our divisor */
        outb(0x43, 0x36);           /* Set our command byte 0x36 */
        outb(0x40, divisor & 0xFF); /* Set low byte of divisor */
        outb(0x40, divisor >> 8);   /* Set high byte of divisor */
    }
    void cli()
    {
        asm("cli");
    }

    void sti()
    {
        asm("sti");
    }
}