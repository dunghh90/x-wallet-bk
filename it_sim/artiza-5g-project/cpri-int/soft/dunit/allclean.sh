#!/bin/sh

echo "************************************************************"
echo "DUnit ALL Clean ********************************************"
echo "************************************************************"

rm -rf ./mruby
rm -rf ./CUnit

rm -rf ./lib/libcunit.so ./lib/libcunit.so.1 ./lib/libmruby.a

./clean.sh
