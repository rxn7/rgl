#!/bin/sh

mkdir -p out
make -f linux.mk
sudo mkdir -p /usr/include/rgl
sudo cp -r include/* /usr/include/rgl/
sudo cp out/* /usr/lib/
