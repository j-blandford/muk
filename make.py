import sys, getopt
import subprocess

solutions = ["kernel", "modules"]

for opt in sys.argv:
    if opt == "clean" or opt == "cbochs" or opt == "cbuild":
        # Clean the solutions
        for project in solutions:
            subprocess.call(["make", "clean"], cwd=project)

        print("/*** FINISHED CLEANING ***/")

    if opt == "build" or opt == "cbuild" or opt == "run" or opt == "bochs" or opt == "cbochs":
        # Compile all of the solutions:
        for project in solutions:
            subprocess.run(["make", "compile"], cwd=project, check=True)

        # build it into an .iso
        subprocess.call(["grub-mkrescue /usr/lib/grub/i386-pc -o muk.iso build"], shell=True)

        print("/*** FINISHED BUILDING ***/")

    if opt == "run":
        # run with Qemu
        subprocess.call(["qemu-system-i386 \
                -s \
                -cdrom muk.iso \
                -m 32 \
                -drive file=disk.img,format=raw \
                -boot d \
                -monitor stdio \
                -vga cirrus \
                -rtc base=localtime"], shell=True)
    
    if opt == "bochs" or opt == "cbochs":
        subprocess.call(["bochs"], shell=True)

