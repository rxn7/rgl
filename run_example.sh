#!/bin/sh

list_examples() {
	echo "List of examples:"
	echo "=-=-=-=-=-=-=-=-="
	ls examples
}

if [ $# -ne 1 ]
then
	echo "You need to specify the example to run i.e. ./run_example.sh <example's name>"
	list_examples
	exit
fi

if [ ! -d "examples/$1" ]
then
	echo "Example '$1' does not exist."
	list_examples
	exit
fi

make -f linux.mk install
if [ $? -ne 0 ]
then
	echo "Failed to compile RGL, please submit a bug report to https://github.com/rxtthin/rgl/issues"
	exit
fi

cd examples/$1
make
if [ $? -ne 0 ]
then
	echo "Failed to compile example '$1', please submit a bug report to https://github.com/rxtthin/rgl/issues"
	cd ../..
	exit
fi

./$1
cd ../..
