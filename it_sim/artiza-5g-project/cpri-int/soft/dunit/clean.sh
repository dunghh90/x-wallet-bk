#!/bin/sh

echo "************************************************************"
echo "DUnit Clean ***********************************************"
echo "************************************************************"

cd ./ct_lib
make clean 1>/dev/null
cd ../

cd ./ct_lib_dmy
make clean 1>/dev/null
cd ../

cd ./ct_bpf
make clean 1>/dev/null
cd ../

cd ./ct_cmn
make clean 1>/dev/null
cd ../

cd ./ct_du_init
make clean 1>/dev/null
cd ../

cd ./ct_du_initmw
make clean 1>/dev/null
cd ../

cd ./ct_du_pf_com
make clean 1>/dev/null
cd ../

cd ./ct_du_pf
make clean 1>/dev/null
cd ../

cd ./
make clean 1>/dev/null

rm -rf ./lib/libct_*.so
