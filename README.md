# &mu;k ("muk")

A very basic microkernel. This isn't meant to be a full-featured kernel+OS, but as a test bed for understanding and implementing task scheduling (with daemon processes) and using ramdisks.

![muk](https://raw.githubusercontent.com/prasoc/muk/master/pic-1.png)

## Current Features
* Written in C++ and a small amount of ASM
    * A very fragile, tiny version of libstd++ is included
		* std::vector
		* std::string
		* [TODO] std::bitset
* GRUB2 bootloader
* Page tables
* Interrupts
    * Timer IRQ for sleep() and wait()
	* Keyboard IRQ for TTY input
	* [TODO] Exception handling
* Higher-half kernel loading
* Basic time-slicing scheduler
    * Multiple threads can run concurrently!
	* [TODO] thread priorities
* Starting to implement a POSIX-compatible terminal
* Filesystems
    * [TODO] FAT16
	* [TODO] ext2
