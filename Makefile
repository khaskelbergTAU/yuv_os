.PHONY: iso kernel qemu qemu-gdb qemu-kernel qemu-test clean

iso: kernel grub.cfg
	cp kernel/kernel.elf isodir/boot/kernel.elf
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir


kernel:
	$(MAKE) -C kernel


grub: kernel
	qemu-system-i386.exe -serial stdio -cdrom myos.iso

run: kernel
	qemu-system-i386.exe -serial stdio -kernel kernel/kernel.elf

debug: kernel
	qemu-system-i386.exe -nographic -kernel kernel/kernel.elf

clean:
	$(MAKE) clean -C kernel
	rm -f iso/boot/kernel.elf
	rm -f os.iso

