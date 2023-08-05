#include "interrupts.h"
namespace interrupts
{
    gate_descriptor_t IDT[IDT_SIZE]{};
    void init_interrupts()
    {
        IDT[DIV_ERROR] = gate_descriptor_t(reinterpret_cast<uintptr_t>(irq_handlers::handle_divide_by_zero), gate_type_e::INTERRUPT_32);
       _load_idt(IDT, sizeof(IDT) - 1);
    }
}