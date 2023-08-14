#include "handlers.h"
char kbd_US [128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
  '\t', /* <-- Tab */
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    0, /* <-- control key */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};
namespace irq_handlers
{
    using namespace serial;
    static void print_frame(interrupt_frame_t * frame)
    {
        DEBUG_PORT.printf("interrupt_frame_t *\n{\n\teflags:\t%b.:64\n\tcs:\t%x\n\teip:\t%p\n}\n", frame->flags, frame->cs, frame->eip);
    }
    const uint16_t ps2_data_port = 0x60;
    static inline void pic_EOI(uint32_t irq)
    {
        using namespace inline_asm;
        const uint8_t PIC_EOI = 0x20;
        if (irq >= 8)
            outb(0x21, PIC_EOI);
        outb(0x20, PIC_EOI);
    }
    void __attribute__((interrupt)) handle_divide_by_zero(interrupt_frame_t * frame)
    {
        print_frame(frame);
        DEBUG_PORT.printf("what are you doing dividing by zero???\n");
        kernel_panic("div by zero :(");
    }
    void __attribute__((interrupt)) handle_timer(interrupt_frame_t * /* frame */)
    {
        /*
        static bool on = false;
        screen.set_cursor(on);
        on = not on;
        */ // blink cursor example

        // do nothing for now.
        pic_EOI(TIMER);
    }
    void __attribute__((interrupt)) handle_keyboard(interrupt_frame_t * /* frame */)
    {
        uint8_t scancode;
        scancode = inline_asm::inb(ps2_data_port);
        if(scancode < 0x80)
        {
            screen.putc(kbd_US[scancode]);
            //DEBUG_PORT.printf("scancode %x\n", scancode);
        }
        pic_EOI(KEYBOARD);
    }

    void __attribute__((interrupt)) handle_double_fault(interrupt_frame_t * frame)
    {
        print_frame(frame);
        DEBUG_PORT.printf("Oops! double fault!!\n");
        kernel_panic("double fault.");
    }
    void __attribute__((interrupt)) handle_gen_prot_fault(interrupt_frame_t *  frame, uint64_t error_code)
    {
        print_frame(frame);
        DEBUG_PORT.printf("Oops! general protection fault!! segment selector %x\n", error_code);
        kernel_panic("general protection fault.");
    }
    void __attribute((interrupt)) handle_page_fault(interrupt_frame_t * frame, uint64_t error_code)
    {
        print_frame(frame);
        DEBUG_PORT.printf("page fault!! the error code is %x\n", error_code);
    }
}