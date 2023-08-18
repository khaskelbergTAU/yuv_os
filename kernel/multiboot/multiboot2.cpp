#include "multiboot2.h"

static uint32_t round_up_8(uint32_t num)
{
    return ((num + 7) & (-8));
}

static void parse_tag_type(boot_info *info, multiboot_tag *tag)
{
    switch(tag->type)
    {
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
            info->meminfo = *reinterpret_cast<multiboot_tag_basic_meminfo *>(tag);
        } break;
        case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
        {
            info->elf_sections = *reinterpret_cast<multiboot_tag_elf_sections *>(tag);
        } break;
    }
}

boot_info::boot_info(multiboot_info *info)
{
    uint32_t header_size = info->total_size;
    multiboot_tag *curr_tag = reinterpret_cast<multiboot_tag *>(info + 1);
    while(curr_tag->type != 0)
    {
        uint32_t rounded = round_up_8(curr_tag->size);
        header_size -= rounded;
        curr_tag += rounded >> 3;
        parse_tag_type(this, curr_tag);
    }
}