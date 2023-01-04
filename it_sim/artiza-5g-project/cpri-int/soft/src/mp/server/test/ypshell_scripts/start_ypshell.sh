#!/bin/sh

if [ $# -lt 1 ]; then
yp-shell --timeout=0 --log-level=debug2
else
yp-shell --timeout=0 --log-level=info --log=/tmp/ypshell.log --run-script=$1
fi
