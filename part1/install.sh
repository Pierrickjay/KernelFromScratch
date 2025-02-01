#!/bin/bash

USER=`whoami`

do_install() {
    # KVM
    sudo apt install qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils -y
    sudo adduser ${USER} libvirt
    sudo adduser ${USER} kvm
    # # GRUB
    # sudo apt install grub-common
    # ASM
    sudo apt install nasm
    sudo apt-get install xorriso
}


sudo apt update
do_install