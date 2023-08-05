#ifndef _KERNEL_INTERRUPTS_IRQ_HANDLERS_HANDLERS_H
#define _KERNEL_INTERRUPTS_IRQ_HANDLERS_HANDLERS_H
#include <stdint.h>
#include "serial.h"
#include "panic.h"
namespace irq_handlers
{
    struct __attribute__((packed)) interrupt_frame_t
    {
        uint32_t eip;
        uint32_t cs;
        uint32_t flags;
        uint32_t sp;
        uint32_t ss;
    };
    void handle_divide_by_zero(interrupt_frame_t *frame);
}


#endif/* _KERNEL_INTERRUPTS_IRQ_HANDLERS_HANDLERS_H */
