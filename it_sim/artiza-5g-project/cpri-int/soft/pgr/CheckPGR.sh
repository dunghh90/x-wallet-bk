#!/bin/sh

find ../src/ -type f -name *.c.ao -print | xargs rm -f
rm -rf *.log *.csv

if [ "$1" = "mw" -o "$1" = "" ] ; then
make -f Makepgr_mw
fi

if [ "$1" = "cmn" -o "$1" = "" ] ; then
make -f Makepgr_cmn
fi

if [ "$1" = "init" -o "$1" = "" ] ; then
make -f Makepgr_init
fi

if [ "$1" = "pf" -o "$1" = "" ] ; then
make -f Makepgr_pf
fi

if [ "$1" = "tools" -o "$1" = "" ] ; then
make -f Makepgr_tools
fi

if [ "$1" = "rfic" -o "$1" = "" ] ; then
make -f Makepgr_rfic
fi

if [ "$1" = "mp" -o "$1" = "" ] ; then
make -f Makepgr_mp
fi
