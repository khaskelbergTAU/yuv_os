#!/bin/bash

QEMU=qemu-system-i386

DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
case "$1" in
    "build" )
    cmake --build $DIR/build;;
    "configure" )
    shift 1
    cd $DIR/build
    rm -rf *
    cmake -DCMAKE_TOOLCHAIN_FILE=$DIR/toolchain.cmake -DCMAKE_CXX_COMPILER_WORKS=1 -DCMAKE_C_COMPILER_WORKS=1 $@ $DIR
    ;;
    "run" )
    case "$2" in
        "gui" )
	    $QEMU -serial stdio -kernel $DIR/build/kernel/kernel.elf;;
        "nogui" )
	    $QEMU -s -S -nographic -kernel $DIR/build/kernel/kernel.elf;;
        "disk" )
	    cp $DIR/kernel/kernel.elf $DIR/isodir/boot/kernel.elf
	    cp $DIR/grub.cfg $DIR/isodir/boot/grub/grub.cfg
	    grub-mkrescue -o $DIR/build/myos.iso $DIR/isodir
	    $QEMU -serial stdio -cdrom $DIR/myos.iso
        ;;
        *)
        echo "Usage: $0 run <gui|nogui|disk>"
        exit -1;;
    esac
    ;;
    *)
    echo "Usage: $0 <build|configure|run>"
    exit -1
    ;;
esac

