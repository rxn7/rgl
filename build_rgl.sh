#/bin/bash

./build_shaders.sh

echo "==========================="
echo "Compiling RGL for linux..."
make -f linux.mk

if [ $? -ne 0 ]
then
	echo "Failed to compile RGL for linux, please submit a bug report to https://github.com/rxtthin/rgl/issues"
	exit
fi
