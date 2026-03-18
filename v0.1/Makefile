CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m32 -ffreestanding -O2 -nostdlib -T linker.ld

all: denemOS.iso

src/boot.o: src/boot.s
	$(AS) src/boot.s -o src/boot.o

src/kernel.o: src/kernel.c
	$(CC) $(CFLAGS) -c src/kernel.c -o src/kernel.o

denemOS: src/boot.o src/kernel.o
	$(LD) $(LDFLAGS) -o denemOS src/boot.o src/kernel.o

denemOS.iso: denemOS
	mkdir -p isodir/boot/grub
	cp denemOS isodir/boot/denemOS
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o denemOS.iso isodir

clean:
	rm -f denemOS denemOS.iso src/*.o
	rm -rf isodir
