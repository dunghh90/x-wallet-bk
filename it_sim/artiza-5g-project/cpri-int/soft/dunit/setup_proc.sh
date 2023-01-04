#!/bin/sh

if [ "$1" = "lib" ] ; then
librrh=./ct_lib
elif [ "$1" = "lib_dmy" ] ; then
librrh=./ct_lib_dmy
elif [ "$1" = "bpf" ] ; then
librrh=./ct_bpf
elif [ "$1" = "cmn" ] ; then
librrh=./ct_cmn
elif [ "$1" = "init" ] ; then
librrh=./ct_du_init
elif [ "$1" = "initmw" ] ; then
librrh=./ct_du_initmw
elif [ "$1" = "pf" ] ; then
librrh=./ct_du_pf
elif [ "$1" = "mp" ] ; then
librrh=./ct_du_mp
elif [ "$1" = "pf_com" ] ; then
librrh=./ct_du_pf_com
elif [ "$1" = "units" ] ; then
librrh=./ct_units
else
 exit 1
fi

echo "************************************************************"
echo "$1 Proc Setup ********************************************"
echo "************************************************************"
cd $librrh
make clean 1>/dev/null
make 1>/dev/null
if [ $? -eq 0 ]; then
   echo "... Finish"
else 
   echo "Compile Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi
make install 1>/dev/null
cd ../
