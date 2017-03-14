import sys, getopt

from subprocess import call

solutions = ["kernel"]

for opt in sys.argv:
    if opt == "build" or opt == "run":
        # Compile all of the solutions:
        for project in solutions:
            call(["make", "compile"], cwd=project)

        # build it into an .iso
        call(["grub-mkrescue /usr/lib/grub/i386-pc -o muk.iso build"], shell=True)

        print("/*** FINISHED BUILDING ***/")

    if opt == "run":
        # run with Qemu
        call(["qemu-system-i386 -s -cdrom muk.iso -m 64 -monitor stdio -vga cirrus"], shell=True)

    if opt == "clean":
        # Clean the solutions
        for project in solutions:
            call(["make", "clean"], cwd=project)

        print("/*** FINISHED CLEANING ***/")
