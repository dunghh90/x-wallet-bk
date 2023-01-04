#!/bin/sh

cd ../src make clean 1>/dev/null 2>/dev/null ; cd ../dunit

if [ -d ./lib ]; then
echo "already created lib"
else
mkdir lib
fi

echo "************************************************************"
echo "DUnit Setup Start *****************************************"
echo "************************************************************"
if [ -d ./mruby ]; then
echo "muby unzip skip .."
else
echo "************************************************************"
echo "mruby unzip    *********************************************"
echo "************************************************************"
rm -rf ./mruby 1>/dev/null
tar zxvf ./mruby-1.0.0.tar.gz 1>/dev/null
mv ./mruby-1.0.0 ./mruby
cp -rf ./config/build_config.rb ./mruby/
cp -rf ./config/mrbconf.h ./mruby/include/
fi
if [ -e ./lib/libmruby.a  ]; then
echo "muby setup skip .."
else
echo "************************************************************"
echo "mruby Setup    *********************************************"
echo "************************************************************"
cd ./mruby
make clean 1>/dev/null
make 1>/dev/null
cp -rf ./build/host/lib/libmruby.a ../lib 1>/dev/null
cd ../
fi

if [ ! -d ./CUnit ]; then
bzip2 -dc CUnit-2.1-2-src.tar.bz2 | tar xvf -
mv ./CUnit-2.1-2 ./CUnit
rm -rf CUnit-2.1-2
cd ./CUnit
#./configure --enable-curses --enable-pic
./configure
make
cp ./CUnit/Sources/.libs/libcunit.so ../lib/
cd ../lib/
ln -s libcunit.so libcunit.so.1
cd ../
fi

./setup_proc.sh units
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi

./setup_proc.sh lib
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi

./setup_proc.sh lib_dmy
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi

./setup_proc.sh bpf
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi

./setup_proc.sh cmn
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi

./setup_proc.sh pf_com
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi

./setup_proc.sh init
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi

./setup_proc.sh initmw
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi


./setup_proc.sh pf
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi

./setup_proc.sh mp
if [ $? -ne 0 ]; then
   echo "Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi



echo "************************************************************"
echo "DUnit Core Setup  *****************************************"
echo "************************************************************"
make clean 1>/dev/null
make 1>/dev/null
if [ $? -eq 0 ]; then
   echo "... Finish"
else 
   echo "Compile Error !!!!!!!!!!!!!!!!!!!!"
   exit 1
fi
make install 1>/dev/null
ls -l ./bin
echo "************************************************************"
echo "Let's go to DU Code Test !!!                           ****"
echo "************************************************************"

