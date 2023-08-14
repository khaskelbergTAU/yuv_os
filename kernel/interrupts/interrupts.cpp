#include "interrupts.h"
namespace interrupts
{
    using namespace irq_handlers;
    gate_descriptor_t IDT[IDT_SIZE]{};
    static void register_routine(uint32_t index, void (*handler)(interrupt_frame_t *), gate_type_e gate_type)
    {
        IDT[index] = gate_descriptor_t(reinterpret_cast<uintptr_t>(handler), gate_type);
    }
    static void register_routine_error(uint32_t index, void (*handler)(interrupt_frame_t *, uint64_t), gate_type_e gate_type)
    {
        IDT[index] = gate_descriptor_t(reinterpret_cast<uintptr_t>(handler), gate_type);
    }
    void init_interrupts()
    {
        register_routine(DIV_ERROR, handle_divide_by_zero, TRAP);
        register_routine(TIMER, handle_timer, INTERRUPT);
        register_routine(KEYBOARD, handle_keyboard, INTERRUPT);
        register_routine(DOUBLE_FAULT, handle_double_fault, TRAP);
        register_routine_error(GENERAL_PROTECTION_FAULT, handle_gen_prot_fault, TRAP);
        register_routine_error(PAGE_FAULT, handle_page_fault, TRAP);
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