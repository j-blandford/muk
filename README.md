# &mu;k ("muk")

A very basic microkernel. This isn't meant to be a full-featured kernel+OS, but as a test bed for understanding and implementing task scheduling (with daemon processes) and using ramdisks.

![muk](https://raw.githubusercontent.com/prasoc/muk/master/pic-1.png)

## Current Features
* Written in C++ and a small amount of ASM
    * A very fragile, tiny version of libstd++ is included
		* std::vector
		* std::string
		* std::bitset
        * std::find / find_if / move / copy
        * [WIP] std::array
* GRUB2 bootloader
* Page tables
    * [TODO] processes have own CR3
* Memory management
    * Virtual Memory management
    * Heap manager (kernel only for the moment)
    * Physical Memory management
* Interrupts
    * Timer IRQ for sleep() and wait()
	* Keyboard IRQ for TTY input
	* Lock/Unlock the scheduler per thread
	* [TODO] Exception handling
* Higher-half kernel loading
* Time-slicing Scheduler
    * Multiple threads can run concurrently
        * Kernel is completely multithreaded
    * Per-thread stack space
	* [TODO] thread priorities
    * [TODO] per-process Page tables 
* Starting to implement a TTY (future: POSIX compatibility)
* Filesystems
    * FAT16 (almost finished)
    * [TODO] ext2
* libcxxrt ported
    * allows C++ to use RTTI (for typeid and dynamic_cast<T>)
    * try/catch exception handling is nearly implemented!
