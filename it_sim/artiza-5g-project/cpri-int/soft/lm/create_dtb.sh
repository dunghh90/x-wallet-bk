#!/bin/sh

echo "Clean ..."
sudo rm -rf ../dtb/system.dtb
sudo rm -rf ../dtb/system_qemu.dtb

echo "Create DTB file ..."
dtc -I dts -O dtb -o ../dtb/system.dtb ../src/kernel/zynqU/linux/arch/arm64/boot/dts/system.dts
dtc -I dts -O dtb -o ../dtb/system_qemu.dtb ../src/kernel/zynqU/linux/arch/arm64/boot/dts/system_qemu.dts

echo "Create Result ..."
ls -la ../dtb/
