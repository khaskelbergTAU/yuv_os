#include "handlers.h"
namespace irq_handlers
{
    using namespace serial;
    void __attribute__((interrupt)) handle_divide_by_zero(interrupt_frame_t *frame)
    {
        DEBUG_PORT.printf("what are you doing dividing by zero??? %x\n", *frame);
        kernel_panic();
    }
}