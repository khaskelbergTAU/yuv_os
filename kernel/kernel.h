#ifndef _KERNEL_KERNEL_H
#define _KERNEL_KERNEL_H

#include "video.h"
#include "serial.h"
#include "memory/paging.h"
#include "memory/gdt.h"
#include "utils/debug.h"
#include "utils/string.h"
#include "interrupts.h"
#include "asm.h"

extern "C"
{
    char __kernel_start;
    char __kernel_end;
}


#endif/* _KERNEL_KERNEL_H */
