NASM=/usr/bin/nasm
DD=/bin/dd
CAT=/bin/cat
QEMU=/usr/bin/qemu-system-x86_64
boot.bin: boot.asm
	$(NASM) -f bin boot.asm -o boot.bin
boot.flp: boot.bin
	$(CAT) boot.bin /dev/zero | $(DD) bs=512 count=2880 of=boot.flp
run: boot.flp
	$(QEMU) -drive file=boot.flp,format=raw,index=0,if=floppy
clean:
	rm -rf boot.bin
