cmake_minimum_required(VERSION 3.18)


set(CMAKE_CXX_STANDARD 20)
project(yuvOS)

enable_language(ASM_NASM)

# custom nasm flags
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -f elf64 -g -o <OBJECT> <SOURCE>")
set(OPT_LEVEL -O0)

add_compile_options(-ffreestanding ${OPT_LEVEL} -Wall -Wextra -fno-exceptions -fno-rtti -fno-plt -fno-pic -Werror -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -g -fdiagnostics-color=always -mcmodel=kernel -fno-pie -no-pie)
add_link_options(-T ${PROJECT_SOURCE_DIR}/kernel/linker.ld -ffreestanding ${OPT_LEVEL} -nostdlib)

add_subdirectory(kernel)
