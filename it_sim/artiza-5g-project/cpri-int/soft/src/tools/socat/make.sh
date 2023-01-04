#!/bin/sh

rm -rf socat-1.7.3.3
tar -zxvf socat.tar.gz
cp socat.c ./socat-1.7.3.3/
cd socat-1.7.3.3
make clean 2>/dev/null
make 2>/dev/null
cp socat ../cmd_socat
exit
