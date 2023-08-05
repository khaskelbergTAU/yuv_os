#!/bin/bash

QEMU=qemu-system-i386

DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
while ! [[ -z $@ ]]
do
echo running stage $1
case "$1" in
    "build" )
    shift 1
    cmake --build $DIR/build;;
    "configure" )
    shift 1
    cd $DIR/build
    rm -rf *
    cmake -DCMAKE_TOOLCHAIN_FILE=$DIR/toolchain.cmake -DCMAKE_CXX_COMPILER_WORKS=1 -DCMAKE_C_COMPILER_WORKS=1 -GNinja $DIR
    ;;
    "run" )
    case "$2" in
        "gui" )
        shift 2
	    $QEMU -serial stdio -kernel $DIR/build/kernel/kernel.elf;;
        "nogui" )
        shift 2
	    $QEMU -nographic -kernel $DIR/build/kernel/kernel.elf;;
        "disk" )
        shift 2
	    cp $DIR/build/kernel/kernel.elf $DIR/isodir/boot/kernel.elf
	    cp $DIR/grub.cfg $DIR/isodir/boot/grub/grub.cfg
	    grub-mkrescue -o $DIR/build/myos.iso $DIR/isodir
	    $QEMU -serial stdio -cdrom $DIR/build/myos.iso
        ;;
        *)
        echo "Usage: $0 run <gui|nogui|disk>"
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

