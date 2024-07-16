# yuv_os

just me trying (poorly) to write a kernel.

if youre here for some reason, have fun and good luck understanding this plate of spaghetti!

## configuration

    build a cross compiler as detailed in https://wiki.osdev.org/GCC_Cross-Compiler
    dont forget to enable no-red-zone!
    re-build crtbegin.o and crtbegin.end with -mcmodel=large

    apt install grub-common xorriso grub-pc-bin

    hopefully it should work now?
