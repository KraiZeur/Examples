# What is U-Boot

U-Boot is an open-source, primary boot loader used in embedded devices to package the instructions to boot the device's operating system kernel. It is available for a number of computer architectures, including 68k, ARM, Blackfin, MicroBlaze, MIPS, Nios, SuperH, PPC, RISC-V and x86.

# U-Boot Git Project

## Compiling U-Boot

Compiling U-Boot for platform vexpress_ca9x4

~~~~
wget ftp://ftp.denx.de/pub/u-boot/u-boot-2020.01.tar.bz2
make vexpress_ca9x4_config ARCH=arm CROSS_COMPILE=arm-none-eabi-
make all ARCH=arm CROSS_COMPILE=arm-none-eabi-
~~~~

## In-Depth View of U-Boot Folder Hierarchy

* ./arch/arm/dts
  * This Folder contains the device Tree Sources for supported platforms
  * How is it included ? ./arch/arm/dts/Makefile => dtb-$(CONFIG_TARGET_VEXPRESS_CA9X4) += vexpress-v2p-ca9.dtb
* ./configs/
  * This Folder contains configuration for supported platforms
  * mainly defines BOOTCOMAND, TEXT_BASE, BAUDRATE, SMC911X_BASE

vexpress-v2p-ca9.dts
vexpress-v2m.dtsi


# Running U-Boot in QEMU

Display QEMU supported emulated machine type

~~~~
qemu-system-arm -M ?
~~~~

## Without Network Support

**Command**
~~~~
qemu-system-arm -M vexpress-a9 -m 32M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel u-boot-2020.01/u-boot
~~~~


*Using printenv inside u-boot you can see predefined variables baudrate=38400 bootcommand=run mmc_elf_bootcmd*

## With Network Support

**Command**

~~~~
qemu-system-arm -M vexpress-a9 -m 32M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel u-boot-2020.01/u-boot -net nic -net tap,ifname=tap0,script=no,downscript=no
~~~~


# Loading images in U-Boot emulated via QEMU

## TFTP

**Prerequisites**

### Installing TFTP server

~~~~
sudo apt-get install xinetd tftpd tftp
~~~~

Create and edit /etc/xinetd.d/tftp

~~~~
sudo vim /etc/xinetd.d/tftp

service tftp
{
    protocol        = udp
    port            = 69
    socket_type     = dgram
    wait            = yes
    user            = nobody
    server          = /usr/sbin/in.tftpd
    server_args     = /tftpboot
    disable         = no
}
~~~~

Create the hosting folder

~~~~
sudo mkdir /tftpboot
sudo chmod -R 777 /tftpboot
sudo chown -R nobody /tftpboot

sudo service xinetd restart
~~~~

### Setting tap0 interface

In order to make tftp working on the host tap0 interface must be set and working.

~~~~
sudo tunctl -t tap0 -u 1000
sudo ip addr add 192.168.1.1/24 dev tap0
sudo ip link set tap0 up
~~~~

**Loading image using tftp from host**
~~~~
setenv ipaddr 192.168.1.100
setenv serverip 192.168.1.1
setenv netmask 255.255.255.0
ping 192.168.1.1
tftpboot 0x60000000 my_image.bin
bootm
~~~~

*0x60000000 is the base address for DDR memory as seen in specification for the vexpress-a9 machine https://developer.arm.com/docs/dui0448/latest/programmers-model/daughterboard-memory-map*

*It is possible to bypass the address destination by using an image already containing the destination addr*

## MMC

# Images format

## Legacy Image

## Single component uImage

An image file that has a U-Boot wrapper (installed by the mkimage utility) that includes the OS type and loader information.

## Monolithic image

## FIT Image

Enhances hash integrity of images with sha1, md5, etc.
Better solution for multi component images.

Ainsi, une FIT image permet de ne disposer que d’un seul fichier, qui lui-même encapsule un ensemble de binaires. Donc plus besoin de charger plusieurs fichiers ! Plutôt intéressant quand même.
Par définition, l’extension pour une FIT image non compilée sera sous la forme .its pour Image Tree Source.


# U-Boot Commands

* iminfo
* printenv
* bdinfo
* fdt

# U-Boot Tools

* mkimage
  * mkimage -l <image> => will display informations about the image (equivalent to iminfo in u-boot)
# Work in Progress

Booting a Single Component Image ::

u-boot loads uImage into memory
parses uImage, copies payload into load addr
jumps to the entry point
boot memory


$ mkimage-l arch/arm/boot/uImage
=> Display size and load addr

FIT = Flatenned Image Tree
FDT = Flatenned Device Tree

The Device Tree 
is a data structure for describing hardware. Rather than hard coding every detail of a device into an operating system, many aspect of the hardware can be described in a data structure that is passed to the operating system at boot time. The device tree is used both by Open Firmware, and in the standalone Flattened Device Tree (FDT) form.

dtb = device tree blob

*.its = Image Source File


$ mkimage-f kernel_fdt.its kernel_fdt.itb => Build the FIT image using mkimage

u-boot load FIT

fitfdt=/boot/kernel_fdt.itb
run mmcargs;
ext2load mmc${mmcdev}:2 ${loadaddr} ${fitfdt};

bootm${loadaddr};


Device Tree :

1/ Basic inside u-boot
2/ COmplete inside the FIT image


QEMU ::
-------------




 u-boot-2018.09/tools/mkimage -A arm -C none -T kernel -a 0x60000000 -e0x60000000 -d better-hang.elf bare-arm.uimg

 sudo qemu-system-arm -M versatilepb -m 128M -kernel u-boot.bin -net nic -net tap,ifname=tap0,script=no,downscript=no

qemu-system-arm -M vexpress-a9 -m 128M -cpu cortex-a9 -nographic -kernel u-boot-2020.01/u-boot -net nic -net user,tftp=$PWD

-machine dumpdtb=dtb.dtb

U-Boot ::
----------
@TODO ::

2/ Créer une image bootable via u-boot (avec chargement depuis tftp)
3/ Créer une image FIT

4/ Comprendre les possibilité de mkimage/uboot (options debug)


https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842374/U-Boot+Images

The TRM for our device is available fromARM, and it indeed contains a memory map. (Note: when working with any device, downloading aPDF version of the TRM is a very good idea.) In this memory map, we can see that the device’s RAM(denoted as “local DDR2”) begins at0x60000000.

mkimage permet de définir l'adresse de chargement de l'image noyau


U-Boot Images ::

Legacy Images
Root File system
FIT