#!/usr/bin/sh

cd examples/$1
make
./$1
cd ../..
