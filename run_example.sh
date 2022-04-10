#!/bin/sh

source ./shell_colors.sh

list_examples() {
	echo "==========================="
	echo "List of examples:"
	ls examples
}

if [ "$#" -ne "1" ]
then
	echo "==========================="
	echoColor $RED "You need to specify the example to run i.e. ./run_example.sh <example's name>"
	list_examples
	exit
fi

if [ ! -d "examples/$1" ]
then
	echo "==========================="
	echoColor $RED "Example '$1' does not exist."
	list_examples
	exit
fi

if [ ! -f /usr/lib/librgl.so ] 
then
	./build_rgl.sh
	if [ $? -ne 0 ]
	then
		exit 1
	fi
fi

echo "==========================="
echo "Comiling example $1..."

cd examples/$1
make
if [ $? -ne 0 ]
then
	echo "==========================="
	echoColor $RED "Failed to compile example '$1', please submit a bug report to https://github.com/rxtthin/rgl/issues"
else
	echo "==========================="
	echoColor $GREEN "Example $1 compiled successfully..."
	echoColor $GREEN "Running example $1..."
	echo "==========================="
	./$1
fi

cd ../..
