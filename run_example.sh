#!/bin/sh

list_examples() {
	echo "==========================="
	echo "List of examples:"
	ls examples
}

if [ $# -ne 1 ]
then
	echo "==========================="
	echo "You need to specify the example to run i.e. ./run_example.sh <example's name>"
	list_examples
	exit
fi

if [ ! -d "examples/$1" ]
then
	echo "==========================="
	echo "Example '$1' does not exist."
	list_examples
	exit
fi

./build_rgl.sh
if [ $? -ne 0 ]
then
	exit -1
fi

echo "==========================="
echo "Comiling example $1..."

cd examples/$1
make
if [ $? -ne 0 ]
then
	echo "==========================="
	echo "Failed to compile example '$1', please submit a bug report to https://github.com/rxtthin/rgl/issues"
else
	echo "==========================="
	echo "Example $1 compiled successfully..."
	echo "Running example $1..."
	echo "==========================="
	./$1
fi

cd ../..
