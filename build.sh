#!/bin/bash

QEMU=qemu-system-x86_64

DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
ISO=$DIR/build/kernel/myos.iso
MEM_AMNT=1G
while ! [[ -z $@ ]]
do
echo running stage $1
case "$1" in
    "build" )
    shift 1
    cmake --build $DIR/build
    ;;
    "configure" )
    shift 1
    cd $DIR/build
    rm -rf $DIR/build/*
    cmake -DCMAKE_TOOLCHAIN_FILE=$DIR/toolchain.cmake -DCMAKE_CXX_COMPILER_WORKS=1 -DCMAKE_C_COMPILER_WORKS=1 -GNinja $@ $DIR
    exit 0
    ;;
    "run" )
    case "$2" in
        "gui" )
        shift 2
	    $QEMU -serial stdio -cdrom $ISO -m $MEM_AMNT ;;
        "nogui" )
        shift 2
	    $QEMU -nographic -cdrom $ISO -m $MEM_AMNT ;;
        "debug" )
        shift 2
	    $QEMU -S -s -serial stdio -cdrom $ISO -m $MEM_AMNT ;;
        *)
        echo "Usage: $0 run <gui|nogui|debug>"
        exit -1;;
    esac
    ;;
    "clean" )
    shift 1
    cmake --build $DIR/build --target clean
    ;;
    *)
    echo "Usage: $0 <build|configure|run|clean>"
    exit -1
    ;;
esac

if [[ $? -ne 0 ]]; then
 exit -1
fi

done

