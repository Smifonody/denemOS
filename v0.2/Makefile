CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
LDFLAGS = -m32 -ffreestanding -O2 -nostdlib -T linker.ld

all: denemOS.iso

src/boot.o: src/boot.s
	$(AS) src/boot.s -o src/boot.o

src/kernel.o: src/kernel.c src/kernel.h src/disk.h src/fs.h
	$(CC) $(CFLAGS) -c src/kernel.c -o src/kernel.o

src/disk.o: src/disk.c src/disk.h src/kernel.h
	$(CC) $(CFLAGS) -c src/disk.c -o src/disk.o

src/fs.o: src/fs.c src/fs.h src/disk.h
	$(CC) $(CFLAGS) -c src/fs.c -o src/fs.o

bin/ls.o: bin/ls.c src/fs.h src/disk.h
	$(CC) $(CFLAGS) -Isrc -c bin/ls.c -o bin/ls.o

denemOS: src/boot.o src/kernel.o src/disk.o src/fs.o bin/ls.o
	$(LD) $(LDFLAGS) -o denemOS src/boot.o src/kernel.o src/disk.o src/fs.o bin/ls.o

denemOS.iso: denemOS
	mkdir -p isodir/boot/grub
	cp denemOS isodir/boot/denemOS
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o denemOS.iso isodir

hdd.img:
	dd if=/dev/zero of=hdd.img bs=512 count=65536

clean:
	rm -f denemOS denemOS.iso src/*.o bin/*.o
	rm -rf isodir