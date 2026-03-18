cd barebones-toolchain
. ./setenv.sh
cd ..
make clean
make
qemu-system-i386 -cdrom denemOS.iso 
