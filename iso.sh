#!/bin/sh
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
set -e
. ./build.sh
 
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
 
cp sysroot/boot/cog.kernel isodir/boot/cog.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "cog" {
	multiboot /boot/cog.kernel
}
EOF
grub-mkrescue -o cog.iso isodir
