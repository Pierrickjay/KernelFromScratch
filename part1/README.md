## KERNEL FROM SCRATCH

# To do

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

# To install 

- KVM : kernel virtual machine 
    - `egrep -c '(vmx|svm)' /proc/cpuinfo` have to return a non 0 
    - `sudo kvm-ok` output should say that u can run kvm acceleration
        - If error install cpu-checker `sudo apt install cpu-checker`
