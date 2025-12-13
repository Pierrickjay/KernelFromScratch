#!/bin/bash

USER=`whoami`

do_install() {
    # KVM
    sudo apt install libvirt-daemon-system libvirt-clients bridge-utils -y
    sudo apt remove qemu-system-x86 -y
    sudo apt autoremove -y
    sudo apt update -y
    sudo apt install qemu-system-x86 qemu-kvm -y
    sudo adduser ${USER} libvirt
    sudo adduser ${USER} kvm
    # # GRUB
    # sudo apt install grub-common
    # ASM
    sudo apt install nasm
    sudo apt-get install xorriso
    unset GTK_PATH # Issue with vs code temrminal
}


sudo apt update
do_install