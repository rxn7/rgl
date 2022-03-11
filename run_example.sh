#!/usr/bin/sh

make -f linux.mk install
cd examples/$1
make
./$1
cd ../..
