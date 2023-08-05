cmake_minimum_required(VERSION 3.18)


set(CMAKE_CXX_STANDARD 20)
project(yuvOS)

enable_language(ASM_NASM)

# custom nasm flags
set(CMAKE_ASM_NASM_COMPILER_FLAGS "-f elf")
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> ${CMAKE_ASM_NASM_COMPILER_FLAGS} -o <OBJECT> <SOURCE>")

add_compile_options(-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-plt -fno-pic -m32 -Werror -mno-red-zone -g)
add_link_options(-T ${PROJECT_SOURCE_DIR}/kernel/linker.ld -ffreestanding -O0 -nostdlib)

add_subdirectory(kernel)
