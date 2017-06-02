import sys, getopt
import subprocess

solutions = [{ "name": "libcxxrt", "build": False },
             { "name": "kernel", "build": True }, 
             { "name": "modules", "build": False }]

for opt in sys.argv:
    if opt == "clean" or opt == "cbochs" or opt == "rebuild":
        # Clean the solutions
        for project in solutions:
            if project['build']:
                subprocess.call(["make", "clean"], cwd=project['name'])

        print("/*** FINISHED CLEANING ***/")

    if opt == "build" or opt == "rebuild" or opt == "run" or opt == "bochs" or opt == "cbochs":
        # Compile all of the solutions:
        for project in solutions:
            if project['build'] or opt == "rebuild":
                subprocess.run(["make", "compile"], cwd=project['name'], check=True)

        # # we've compiled each of our source files, let's link them all together into the kernel.elf
        # subprocess.run(["make", "compile"], check=True)

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

