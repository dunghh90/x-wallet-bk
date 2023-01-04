#
# Makefile for the 25g network device drivers.
#
#

export ARCH=arm64
export CROSS_COMPILE=/home/peta/my-project-wrl-tiny-RT/build/tmp-glibc/work/xilinx_zynqmp-wrs-linux/linux-yocto-rt/4.12.29+gitAUTOINC+195a1f4d65_dce2fd0f04-r0/recipe-sysroot-native/usr/bin/aarch64-wrs-linux/aarch64-wrs-linux-

obj-m := fheth.o

ifdef CROSS_COMPILE
EXTRA_CFLAGS+=-Wall -Wstrict-prototypes

ifndef KERNEL
export KERNEL=/home/peta/my-project-wrl-tiny-RT/build/tmp-glibc/work/xilinx_zynqmp-wrs-linux/linux-yocto-rt/4.12.29+gitAUTOINC+195a1f4d65_dce2fd0f04-r0/linux-xilinx_zynqmp-preempt-rt-build
endif

endif

all:
	make -C $(KERNEL) M=$(PWD) modules
	cp fheth.ko ../../../../../ramdisk/target/opt/lkm/
clean:
	make -C $(KERNEL) M=$(PWD) clean
