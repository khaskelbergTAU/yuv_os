#include "gdt.h"

namespace gdt
{
    gdt_entry_t GDT[GDT_SIZE]{};
    static void init_kernel_code_segment(uint32_t ind)
    {
        gdt_entry_access_t access(0, 1, 0, 1, 1, 0);
        gdt_entry_t entry(0, 0xfffff, 1, 0, 1, access);
        GDT[ind] = entry;
    }
    static void init_kernel_data_segment(uint32_t ind)
    {
        gdt_entry_access_t access(0, 1, 0, 0, 1, 0);
        gdt_entry_t entry(0, 0xfffff, 1, 0, 1, access);
        GDT[ind] = entry;
    }
    static void init_user_code_segment(uint32_t ind)
    {
        gdt_entry_access_t access(0, 1, 0, 1, 1, 3);
        gdt_entry_t entry(0, 0xfffff, 1, 0, 1, access);
        GDT[ind] = entry;
    }
    static void init_user_data_segment(uint32_t ind)
    {
        gdt_entry_access_t access(0, 1, 0, 0, 1, 3);
        gdt_entry_t entry(0, 0xfffff, 1, 0, 1, access);
        GDT[ind] = entry;
    }
    void init_gdt()
    {
        init_kernel_code_segment(KERNEL_CODE_SELECTOR);
        init_kernel_data_segment(KERNEL_DATA_SELECTOR);
        init_user_code_segment(USER_CODE_SELECTOR);
        init_user_data_segment(USER_DATA_SELECTOR);
        // TODO: task state segment
        _load_gdt(GDT, sizeof(GDT) - 1);
        _reload_segments();
    }
} // namespace gdt