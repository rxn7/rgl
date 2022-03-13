#!/usr/bin/sh

if [ $# -eq 0 ]
then
	echo "You need to specify the example to run, e.g. ./run_example.sh balls"
	exit
fi

if [ ! -d "examples/$1" ]
then
	echo "Example '$1' does not exist."
	echo "List of examples: "
	ls examples
	exit
fi

make -f linux.mk install
cd examples/$1
make
./$1
cd ../..
