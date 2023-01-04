#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
secondString=""
result=$(echo "$DIR" | sed "s/src/$secondString/")
PATH_temp="/home/user/bin:/home/user/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:${result}ramdisk/target/opt/du/:${result}ramdisk/target/opt/proc/"

LD_PATH_temp="${result}ramdisk/target/lib64/:${result}ramdisk/target/lib/:${result}ramdisk/target/usr/local/lib64/"

PATH_QEMU_temp="${result}ramdisk"
export PATH=$PATH_temp
export LD_LIBRARY_PATH=$LD_PATH_temp
export PATH_QEMU=$PATH_QEMU_temp
echo $PATH
echo $LD_LIBRARY_PATH
echo $PATH_QEMU
DU_mw
DU_init