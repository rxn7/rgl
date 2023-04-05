#/bin/bash

source ./shell_colors.sh

./build_shaders.sh

mkdir -p out

echo "==========================="
echo "Compiling RGL for linux..."
make -f make/linux.mk

echo "==========================="
echo "Installing RGL"
make -f make/linux.mk install

if [ $? -ne 0 ]
then
	echoColor $RED "Failed to compile RGL for linux, please submit a bug report to https://github.com/rxtthin/rgl/issues"
	exit 1
else
	echoColor $GREEN "Successfully compiled RGL for linux"
fi
