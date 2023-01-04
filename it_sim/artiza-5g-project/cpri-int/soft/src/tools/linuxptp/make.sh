#!/bin/sh

rm -rf linuxptp-master
unzip linuxptp-master.zip
cp diff/* linuxptp-master/
cd linuxptp-master
make
cp ptp4l ../../../../ramdisk/target/opt/debug/ptp4l
#cp nsm ../../../../ramdisk/target/opt/debug/nsm
#cp phc2sys ../../../../ramdisk/target/opt/debug/phc2sys
#cp phc_ctl ../../../../ramdisk/target/opt/debug/phc_ctl
#cp pmc ../../../../ramdisk/target/opt/debug/pmc
#cp timemaster ../../../../ramdisk/target/opt/debug/timemaster
exit
