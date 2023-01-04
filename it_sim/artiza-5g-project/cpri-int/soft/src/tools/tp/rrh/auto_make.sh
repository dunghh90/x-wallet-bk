#!/bin/sh

cd rrhlib
make clean
make
cd CT
make
cd ../../
make clean
make
make install
