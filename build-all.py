from subprocess import call

# Compile the kernel
call(["make", "compile"], cwd="kernel")

# build it into an .iso
call(["grub-mkrescue /usr/lib/grub/i386-pc -o muk.iso build"], shell=True)

# now run with Qemu
call(["qemu-system-i386 -s -cdrom muk.iso -m 64 -monitor stdio -vga cirrus"], shell=True)
