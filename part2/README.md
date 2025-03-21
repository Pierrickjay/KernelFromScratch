# KERNEL FROM SCRATCH

This project aims to build a kernel from scratch, as part of the 42 school curriculum. It is split into 10 progressive parts, with this repository containing "KFS-1" (Kernel From Scratch - Part 1). The goal is to understand operating system fundamentals by implementing a basic kernel from the ground up, starting with boot management and basic screen output.

## Prerequisites

- NASM
- GCC
- GRUB

## How to build

```bash
make
```

The kernel will be built in the `kfs1` directory.

To run the kernel with qemu, use the following command:

```bash
make virtualisation
```

To clean the project, use the following command:

```bash
make clean
```

or

```bash
make fclean
```

to clean the project and the iso file.

# Documentation 
  gdt : http://www.osdever.net/bkerndev/Docs/gdt.htm

# First reflexion

## To do

• Install GRUB on an virtual image
• Write an ASM boot code that handles multiboot header, and use GRUB to init and
call main function of the kernel itself.
• Write basic kernel code of the choosen language.
• Compile it with correct flags, and link it to make it bootable.
• Once all of those steps above are done, you can write some helpers like kernel types
or basic functions (strlen, strcmp, ...)
• Your work must not exceed 10 MB.
• Code the interface between your kernel and the screen.
• Display "42" on the screen.

## To install

- KVM : kernel virtual machine
  - `egrep -c '(vmx|svm)' /proc/cpuinfo` have to return a non 0
  - `sudo kvm-ok` output should say that u can run kvm acceleration
    - If error install cpu-checker `sudo apt install cpu-checker`

## To verify

grub-file --is-x86-multiboot kfs1
