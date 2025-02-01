#!/bin/bash

USER=`whoami`

do_install() {
    # KVM
    sudo apt install qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils -y
    sudo adduser ${USER} libvirt
    sudo adduser ${USER} kvm
}


sudo apt update
do_install