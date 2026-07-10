TARGET = FreeUnix.iso

CC_FLAGS = -c -I./ -m32 -ffreestanding -nostdlib -fno-stack-protector
AS_FLAGS = -f elf32
LD_FLAGS = -m elf_i386 -T linker.ld -o iso/boot/kernel build/traps.o build/keyboard.o build/printk.o build/gdt.o build/pic.o build/ioport.asm.o build/main.o build/console.o build/panic.o build/entry.asm.o build/sched.o build/signal.o build/alarm.o

CC = gcc $(CC_FLAGS)
AS = nasm $(AS_FLAGS)
LD = ld $(LD_FLAGS)
QEMU = qemu-system-x86_64

all:
	clear
	mkdir -p build
	$(AS) arch/x86/ioport.asm -o build/ioport.asm.o
	$(AS) entry.asm -o build/entry.asm.o

	$(CC) kernel/console.c -o build/console.o
	$(CC) kernel/main.c -o build/main.o
	$(CC) kernel/keyboard.c -o build/keyboard.o
	$(CC) kernel/sched.c -o build/sched.o
	$(CC) kernel/signal.c -o build/signal.o
	$(CC) kernel/alarm.c -o build/alarm.o
	$(CC) kernel/printk.c -o build/printk.o
	$(CC) kernel/panic.c -o build/panic.o

	$(CC) arch/x86/gdt.c -o build/gdt.o
	$(CC) arch/x86/pic.c -o build/pic.o
	$(CC) arch/x86/traps.c -o build/traps.o
	$(LD)
	grub-mkrescue -o $(TARGET) iso/
	$(QEMU) $(TARGET)

clean:
	clear
	rm -rf build/*.o build/*.asm.o
	rm -rf $(TARGET)
