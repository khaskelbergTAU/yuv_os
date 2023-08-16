#include "paging.h"
namespace paging
{

void PageAllocator::map_kernel(uintptr_t kernel_start, uintptr_t kernel_end, uintptr_t kernel_base)
{
    Page_Pool = reinterpret_cast<Page*>(kernel_end);
    kernel_end += PAGE_SIZE * 3; // so we map these three pages aswell
    kernel_start = kernel_start & (~(0xfff)); // page align, just incase
    PML4 = reinterpret_cast<PML4_entry*>(kernel_end);
    PZ(reinterpret_cast<void*>(PML4));
    kernel_end += PAGE_SIZE; // so we map the PML4 table aswell
    uintptr_t phys_kernel_end = kernel_end - kernel_base;
    uintptr_t PDPT_count = 0;
    uintptr_t PD_count = 0;
    uintptr_t PT_count = 0;
    uintptr_t frame_count = 0;
    for (uintptr_t addr = kernel_start, idx = PML4_INDX(addr); addr < kernel_end; addr += PML4_ENTRY_MAP_SIZE)
    {
        PZ(reinterpret_cast<void*>(phys_kernel_end + kernel_base));
        PML4[idx++] = PML4_entry(1, 0, 0, 0, phys_kernel_end, 0);
        phys_kernel_end += PAGE_SIZE;
        PDPT_count++;
        if (addr >= kernel_end - PML4_ENTRY_MAP_SIZE) break;
    }
    PDPT = reinterpret_cast<PDPT_entry*>(PML4 + 512);
    for (uintptr_t addr = kernel_start, idx = PDPT_INDX(addr); addr < kernel_end; addr += PDPT_ENTRY_MAP_SIZE)
    {
        PZ(reinterpret_cast<void*>(phys_kernel_end + kernel_base));
        PDPT[idx++] = PDPT_entry(1, 0, 0, 0, 0, phys_kernel_end, 0);
        phys_kernel_end += PAGE_SIZE;
        PD_count++;
        if (addr >= kernel_end - PDPT_ENTRY_MAP_SIZE) break;
    }
    PD = reinterpret_cast<PD_entry*>(PDPT + 512 * PDPT_count);
    for (uintptr_t addr = kernel_start, idx = PD_INDX(addr); addr < kernel_end; addr += PD_ENTRY_MAP_SIZE)
    {
        PZ(reinterpret_cast<void*>(phys_kernel_end + kernel_base));
        PD[idx++] = PD_entry(1, 0, 0, 0, 0, phys_kernel_end, 0);
        phys_kernel_end += PAGE_SIZE;
        PT_count++;
        if (addr >= kernel_end - PD_ENTRY_MAP_SIZE) break;
    }
    PT = reinterpret_cast<PT_entry*>(PD + 512 * PD_count);
    for (uintptr_t addr = kernel_start, idx = PT_INDX(addr); addr < kernel_end; addr += PAGE_SIZE)
    {
        PZ(reinterpret_cast<void*>(phys_kernel_end + kernel_base));
        PT[idx++] = PT_entry(1, 0, 0, 0, addr - kernel_base, 0);
        frame_count++;
        if (addr >= kernel_end - PAGE_SIZE) break;
    }
    FRAME = reinterpret_cast<uintptr_t>(PT + 512 * PT_count);
    alloc_end = FRAME + 4096 * frame_count;
    _load_cr3(PML4 - (kernel_base >> 3));
    map_addr(0xffffffff800b8000, 0xb8000);
}

void PageAllocator::map_addr(uintptr_t vaddr, uintptr_t paddr)
{
    PDPT_entry *pdpt = get_pml4_entry(PML4, vaddr);
    PD_entry *pd = get_pdpt_entry(pdpt, vaddr);
    PT_entry *pt = get_pd_entry(pd, vaddr);
    if(!map_pt_entry(pt, vaddr, paddr))
    {
        kernel_panic("haha this cant happen\n");
    }
}

uintptr_t PageAllocator::tmp_map(uintptr_t paddr)
{

}

PDPT_entry* PageAllocator::extract_addr(PML4_entry entry)
{
    return reinterpret_cast<PDPT_entry *>((entry.page_table << 12) | (0xffffULL << 48));
}
PD_entry* PageAllocator::extract_addr(PDPT_entry entry)
{
    return reinterpret_cast<PD_entry *>((entry.page_table << 12) | (0xffffULL << 48));
}
PT_entry* PageAllocator::extract_addr(PD_entry entry)
{
    return reinterpret_cast<PT_entry *>((entry.page_table << 12) | (0xffffULL << 48));
}
Page* PageAllocator::extract_addr(PT_entry entry)
{
    return reinterpret_cast<Page *>((entry.page_frame << 12) | (0xffffULL << 48));
}
PDPT_entry* PageAllocator::get_pml4_entry(PML4_entry* pml4, uintptr_t vaddr)
{
    PML4_entry entry = pml4[PML4_INDX(vaddr)];
    if (present(entry)) return extract_addr(entry);
    entry = PML4_entry(1, 0, 0, 0, alloc_end, 0);
    alloc_end += PAGE_SIZE;
    pml4[PML4_INDX(vaddr)] = entry;
    return extract_addr(entry);
}
PD_entry* PageAllocator::get_pdpt_entry(PDPT_entry* pdpt, uintptr_t vaddr)
{
    PDPT_entry entry = pdpt[PDPT_INDX(vaddr)];
    if (present(entry)) return extract_addr(entry);
    entry = PDPT_entry(1, 0, 0, 0, 0, alloc_end, 0);
    alloc_end += PAGE_SIZE;
    pdpt[PDPT_INDX(vaddr)] = entry;
    return extract_addr(entry);
}
PT_entry* PageAllocator::get_pd_entry(PD_entry* pd, uintptr_t vaddr)
{
    PD_entry entry = pd[PD_INDX(vaddr)];
    if (present(entry)) return extract_addr(entry);
    entry = PD_entry(1, 0, 0, 0, 0, alloc_end, 0);
    alloc_end += PAGE_SIZE;
    pd[PD_INDX(vaddr)] = entry;
    return extract_addr(entry);

}
bool PageAllocator::map_pt_entry(PT_entry* pt, uintptr_t vaddr, uintptr_t paddr)
{
    PT_entry entry = pt[PT_INDX(vaddr)];
    if (present(entry)) return false;
    entry = PT_entry(1, 0, 0, 0, paddr, 0);
    pt[PT_INDX(vaddr)] = entry;
    return true;
}

void _load_cr3(PML4_entry pml4[512])
{
    asm("mov %0, %%cr3" :: "r" (pml4));
}
}