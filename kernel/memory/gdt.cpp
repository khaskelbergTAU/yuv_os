#include "gdt.h"

namespace gdt
{
    gdt_entry_t GDT[64]{};
    void init_kernel_code_segment(uint32_t ind)
    {
        gdt_entry_access_t access(0, 1, 0, 1, 1, 0);
        gdt_entry_t entry(0, 0xffff, 0, 1, 1, access);
        GDT[ind] = entry;
    }
    static void init_kernel_data_segment(uint32_t ind)
    {
        gdt_entry_access_t access(0, 1, 0, 0, 1, 0);
        gdt_entry_t entry(0, 0xffff, 0, 1, 1, access);
        GDT[ind] = entry;
    }
    void init_gdt()
    {
        init_kernel_code_segment(1);
        init_kernel_data_segment(2);
        _load_gdt(GDT, sizeof(GDT) - 1);
        // _reload_segments();
    }
} // namespace gdt