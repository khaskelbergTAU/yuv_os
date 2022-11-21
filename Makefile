.PHONY: iso kernel qemu qemu-gdb qemu-kernel qemu-test clean

QEMU:= qemu-system-i386.exe

myos.iso: kernel grub.cfg
	cp kernel/kernel.elf isodir/boot/kernel.elf
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir


kernel:
	$(MAKE) -C kernel


grub: myos.iso
	$(QEMU)  -serial stdio -cdrom myos.iso

run: kernel
	$(QEMU) -serial stdio -kernel kernel/kernel.elf

debug: kernel
	$(QEMU) -s -S -nographic -kernel kernel/kernel.elf

clean:
	$(MAKE) clean -C kernel
	rm -f iso/boot/kernel.elf
	rm -f os.iso

