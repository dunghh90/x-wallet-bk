#!/bin/sh

cd dulib
make clean
make
cd ../
make clean
make
make install
