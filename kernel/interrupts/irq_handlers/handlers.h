#ifndef _KERNEL_INTERRUPTS_IRQ_HANDLERS_HANDLERS_H
#define _KERNEL_INTERRUPTS_IRQ_HANDLERS_HANDLERS_H
#include <stdint.h>
#include "serial.h"
#include "video.h"
#include "panic.h"
#include "asm.h"
namespace irq_handlers
{
    enum irq_num
    {
        DIV_ERROR = 0,
        DEBUG = 1,
        NMI = 2,
        BREAKPOINT = 3,
        OVERFLOW = 4,
        BOUND_RANGE_EXCEEDE = 5,
        INVALID_OPCODE = 6,
        DEVICE_NOT_AVAILABLE = 7,
        DOUBLE_FAULT = 8,
        COPROCESSOR_SEGMENT_OVERRUN = 9,
        INVALID_TSS = 10,
        SEGMENT_NOT_PRESENT = 11,
        STACK_SEGMENT_FAULT = 12,
        GENERAL_PROTECTION_FAULT = 13,
        PAGE_FAULT = 14,
        FLOATING_POINT_EXCEPTION = 16,
        ALIGNMENT_CHECK = 17,

        TIMER = 32,
        KEYBOARD = 33,
    };
    struct __attribute__((packed)) interrupt_frame_t
    {
        uint32_t eip;
        uint32_t cs;
        uint32_t flags;
        uint32_t sp;
        uint32_t ss;
    };
    void handle_divide_by_zero(interrupt_frame_t *frame, uint32_t error_code);
    void handle_timer(interrupt_frame_t *frame);
    void handle_keyboard(interrupt_frame_t *frame);
    void handle_double_fault(interrupt_frame_t * /* frame */, uint32_t error_code);
}


#endif/* _KERNEL_INTERRUPTS_IRQ_HANDLERS_HANDLERS_H */
