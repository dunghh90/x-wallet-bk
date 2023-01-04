#!/bin/sh

echo "Clean ..."
sudo rm -rf rootfs_mnt
sudo rm -rf *.bin
sudo rm -rf uImage uImage.dd
sudo rm -rf *.dtb
sudo rm -rf *.gz

echo "Kernel copy..."
cp ../kernel/uImage ./
dd if=uImage bs=64 skip=1 of=uImage.dd

echo "dtb copy..."
cp ../dtb/system.dtb ./

echo "ramdisk copy..."
cp ../ramdisk/rootfs.cpio.gz ./

sudo mkdir rootfs_mnt/
sudo gunzip -c rootfs.cpio.gz | sudo sh -c 'cd rootfs_mnt/ && cpio -i'

sudo mkdir -p rootfs_mnt/home/root
sudo mkdir -p rootfs_mnt/home/f_5g_du_lls
sudo mkdir -p rootfs_mnt/O-RAN
sudo mkdir -p rootfs_mnt/O-RAN/log
sudo mkdir -p rootfs_mnt/O-RAN/transceiver
sudo mkdir -p rootfs_mnt/O-RAN/PM
sudo mkdir -p rootfs_mnt/O-RAN/beamforming

if [ ! -d "../ramdisk/target/opt/scripts/o-ran" ]; then
 echo "script files copy..."
 git clone -b o-ran-v1.0.0 http://src_getter:src_getter@sayuri.css.exnet.fujitsu.co.jp/gitlab/5G_M-Plane_xRAN/scripts.git ../ramdisk/target/opt/scripts/o-ran/
 rm -rf ../ramdisk/target/opt/scripts/o-ran/.git
 rm -rf ../ramdisk/target/opt/scripts/o-ran/README.md
else
 echo "scripts files copy skip!!"
fi

sudo cp -rf ../ramdisk/target/* rootfs_mnt/
sudo rm -f rootfs_mnt/lib/lib*
cd rootfs_mnt/usr/lib64/
sudo ln -s libyumapro_agt.so.18.10 libyumapro_agt.so
sudo ln -s libyumapro_db-api.so.18.10 libyumapro_db-api.so
sudo ln -s libyumapro_mgr.so.18.10 libyumapro_mgr.so
sudo ln -s libyumapro_ncx.so.18.10 libyumapro_ncx.so
sudo ln -s libyumapro_sil-sa.so.18.10 libyumapro_sil-sa.so
sudo ln -s libyumapro_subsys-pro.so.18.10 libyumapro_subsys-pro.so
sudo ln -s libyumapro_ycli.so.18.10 libyumapro_ycli.so
sudo ln -s libyumapro_ycontrol.so.18.10 libyumapro_ycontrol.so
cd ../../../
sudo cp -rf ../env/petalinux/project-spec/meta-user/recipes-apps/userfs/files/DUstartup.sh rootfs_mnt/etc/init.d/
cd rootfs_mnt/etc/rc3.d/
sudo ln -s ../init.d/DUstartup.sh S90DUstartup.sh
cd ../../../
sudo find rootfs_mnt -type f -name 'README.md' | xargs sudo rm
sudo chmod u+s rootfs_mnt/bin/busybox.nosuid
sudo chmod u+s rootfs_mnt/bin/ps.procps
sudo chmod u+s rootfs_mnt/bin/netstat.net-tools
sudo chmod u+s rootfs_mnt/usr/sbin/tcpdump
sudo chmod u+s rootfs_mnt/opt/tp/rrh
sudo chmod u+s rootfs_mnt/opt/du/du
sudo chmod u+s rootfs_mnt/opt/debug/*
sudo chmod u+s rootfs_mnt/opt/sh/*
sudo chmod u+s rootfs_mnt/opt/scripts/*
sudo chmod u+s rootfs_mnt/usr/bin/yangcli-pro
sudo chmod u+s rootfs_mnt/usr/bin/yp-controller
sudo chmod u+s rootfs_mnt/usr/bin/yp-shell
sudo chmod u+s rootfs_mnt/usr/bin/sil-sa-app
sudo chmod u+s rootfs_mnt/usr/bin/ypwatcher
sudo chmod u+s rootfs_mnt/usr/sbin/netconfd-pro
sudo chmod u+s rootfs_mnt/usr/sbin/netconf-subsystem-pro
echo "Soft version:"$1 > ./version
sudo cp -f ./version rootfs_mnt/home/f_5g_du/

#### delete config files
sudo rm -rf rootfs_mnt/opt/agent/etc/saa_init_data.conf.3.7G
sudo rm -rf rootfs_mnt/opt/agent/etc/saa_init_data.conf.4.5G
sudo rm -rf rootfs_mnt/opt/agent/etc/saa_validate_data.conf.3.7G
sudo rm -rf rootfs_mnt/opt/agent/etc/saa_validate_data.conf.4.5G
sudo rm -rf rootfs_mnt/etc/yumapro/netconfd-pro.conf
sudo rm -rf rootfs_mnt/opt/sh/nonbeam
#### copy config files
sudo cp -f rootfs_mnt/opt/agent/etc/saa_init_data.conf.3.7G.BF rootfs_mnt/opt/agent/etc/saa_init_data.conf.3.7G
sudo cp -f rootfs_mnt/opt/agent/etc/saa_init_data.conf.4.5G.BF rootfs_mnt/opt/agent/etc/saa_init_data.conf.4.5G
sudo cp -f rootfs_mnt/opt/agent/etc/saa_validate_data.conf.3.7G.BF rootfs_mnt/opt/agent/etc/saa_validate_data.conf.3.7G
sudo cp -f rootfs_mnt/opt/agent/etc/saa_validate_data.conf.4.5G.BF rootfs_mnt/opt/agent/etc/saa_validate_data.conf.4.5G
sudo cp -f rootfs_mnt/etc/yumapro/netconfd-pro_beamforming.conf rootfs_mnt/etc/yumapro/netconfd-pro.conf

sudo sh -c 'cd rootfs_mnt/ && find . | cpio -H newc -o' | gzip -9 > rootfs_5gdu.cpio.gz

echo "uboot Fit file create for beamforming soft..."
mkimage -f image.its Fj5gDuCm_Soft_v$1.bin

##### delete config files
#sudo rm -rf rootfs_mnt/opt/agent/etc/saa_init_data.conf.3.7G
#sudo rm -rf rootfs_mnt/opt/agent/etc/saa_init_data.conf.4.5G
#sudo rm -rf rootfs_mnt/opt/agent/etc/saa_validate_data.conf.3.7G
#sudo rm -rf rootfs_mnt/opt/agent/etc/saa_validate_data.conf.4.5G
#sudo rm -rf rootfs_mnt/etc/yumapro/netconfd-pro.conf
#sudo rm -rf rootfs_mnt/opt/sh/nonbeam
##### copy config files
#sudo cp -f rootfs_mnt/opt/agent/etc/saa_init_data.conf.3.7G.nonBF rootfs_mnt/opt/agent/etc/saa_init_data.conf.3.7G
#sudo cp -f rootfs_mnt/opt/agent/etc/saa_init_data.conf.4.5G.nonBF rootfs_mnt/opt/agent/etc/saa_init_data.conf.4.5G
#sudo cp -f rootfs_mnt/opt/agent/etc/saa_validate_data.conf.3.7G.nonBF rootfs_mnt/opt/agent/etc/saa_validate_data.conf.3.7G
#sudo cp -f rootfs_mnt/opt/agent/etc/saa_validate_data.conf.4.5G.nonBF rootfs_mnt/opt/agent/etc/saa_validate_data.conf.4.5G
#sudo cp -f rootfs_mnt/etc/yumapro/netconfd-pro_nonbeamforming.conf rootfs_mnt/etc/yumapro/netconfd-pro.conf
##### create new files
#echo "non-beamforming mode file" > nonbeam
#sudo mv -f nonbeam rootfs_mnt/opt/sh/nonbeam
#
#sudo sh -c 'cd rootfs_mnt/ && find . | cpio -H newc -o' | gzip -9 > rootfs_5gdu.cpio.gz
#
#echo "uboot Fit file create for non beamforming soft..."
#mkimage -f image.its Fj5gDuCm_Soft_v$1_nonBF.bin
#